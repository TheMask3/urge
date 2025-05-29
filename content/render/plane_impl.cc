// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/render/plane_impl.h"

#include <algorithm>

namespace content {

namespace {

void GPUCreatePlaneInternal(renderer::RenderDevice* device, PlaneAgent* agent) {
  agent->batch = renderer::QuadBatch::Make(**device);
  agent->shader_binding = device->GetPipelines()->viewport.CreateBinding();

  Diligent::CreateUniformBuffer(
      **device, sizeof(renderer::Binding_Flat::Params), "plane.flat.uniform",
      &agent->uniform_buffer, Diligent::USAGE_DEFAULT);
}

void GPUDestroyPlaneInternal(PlaneAgent* agent) {
  delete agent;
}

void GPUUpdatePlaneQuadArrayInternal(renderer::RenderDevice* device,
                                     renderer::RenderContext* context,
                                     PlaneAgent* agent,
                                     TextureAgent* texture,
                                     const base::Rect& src_rect,
                                     const base::Vec2i& viewport_size,
                                     const base::Vec2& scale,
                                     const base::Vec2i& origin,
                                     int32_t opacity,
                                     const base::Vec4& color,
                                     const base::Vec4& tone) {
  // Pre-calculate tile dimensions with scaling
  const float item_x =
      std::max(1.0f, static_cast<float>(texture->size.x) * scale.x);
  const float item_y =
      std::max(1.0f, static_cast<float>(texture->size.y) * scale.y);

  // Wrap float into desire value
  auto value_wrap = [](float value, float range) {
    float result = std::fmod(value, range);
    return result < 0 ? result + range : result;
  };

  // Calculate wrapped origin offsets
  const float wrap_ox = value_wrap(static_cast<float>(origin.x), item_x);
  const float wrap_oy = value_wrap(static_cast<float>(origin.y), item_y);

  // Optimized tile calculation using simplified formula
  const float total_x = static_cast<float>(viewport_size.x) + wrap_ox;
  const float total_y = static_cast<float>(viewport_size.y) + wrap_oy;
  const int32_t tile_x = static_cast<int32_t>(std::ceil(total_x / item_x));
  const int32_t tile_y = static_cast<int32_t>(std::ceil(total_y / item_y));

  // Prepare vertex buffer
  const int32_t quad_size = tile_x * tile_y;
  agent->cache.resize(quad_size);
  const base::Vec4 opacity_norm(static_cast<float>(opacity) / 255.0f);

  // Pointer-based vertex writing with accumulative positioning
  renderer::Quad* quad_ptr = agent->cache.data();
  float current_y = -wrap_oy;
  for (int32_t y = 0; y < tile_y; ++y) {
    float current_x = -wrap_ox;
    for (int32_t x = 0; x < tile_x; ++x) {
      // Set vertex properties directly through pointer
      const base::RectF pos(current_x, current_y, item_x, item_y);
      renderer::Quad::SetPositionRect(quad_ptr, pos);
      renderer::Quad::SetTexCoordRect(quad_ptr, src_rect, texture->size);
      renderer::Quad::SetColor(quad_ptr, opacity_norm);

      // Move to next quad using pointer arithmetic
      ++quad_ptr;
      current_x += item_x;  // X-axis accumulation
    }
    current_y += item_y;  // Y-axis accumulation
  }

  device->GetQuadIndex()->Allocate(quad_size);
  agent->quad_size = quad_size;
  agent->batch->QueueWrite(**context, agent->cache.data(), agent->cache.size());

  renderer::Binding_Flat::Params transient_uniform;
  transient_uniform.Color = color;
  transient_uniform.Tone = tone;
  (*context)->UpdateBuffer(agent->uniform_buffer, 0, sizeof(transient_uniform),
                           &transient_uniform,
                           Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void GPUOnViewportRenderingInternal(renderer::RenderDevice* device,
                                    renderer::RenderContext* context,
                                    Diligent::IBuffer** world_binding,
                                    PlaneAgent* agent,
                                    TextureAgent* texture,
                                    int32_t blend_type) {
  auto& pipeline_set = device->GetPipelines()->viewport;
  auto* pipeline =
      pipeline_set.GetPipeline(static_cast<renderer::BlendType>(blend_type));

  // Setup uniform params
  agent->shader_binding->u_transform->Set(*world_binding);
  agent->shader_binding->u_texture->Set(texture->view);
  agent->shader_binding->u_params->Set(agent->uniform_buffer);

  // Apply pipeline state
  (*context)->SetPipelineState(pipeline);
  (*context)->CommitShaderResources(
      **agent->shader_binding,
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // Apply vertex index
  Diligent::IBuffer* const vertex_buffer = **agent->batch;
  (*context)->SetVertexBuffers(
      0, 1, &vertex_buffer, nullptr,
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
  (*context)->SetIndexBuffer(
      **device->GetQuadIndex(), 0,
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // Execute render command
  Diligent::DrawIndexedAttribs draw_indexed_attribs;
  draw_indexed_attribs.NumIndices = 6 * agent->cache.size();
  draw_indexed_attribs.IndexType = renderer::QuadIndexCache::kValueType;
  (*context)->DrawIndexed(draw_indexed_attribs);
}

}  // namespace

scoped_refptr<Plane> Plane::New(ExecutionContext* execution_context,
                                scoped_refptr<Viewport> viewport,
                                ExceptionState& exception_state) {
  return new PlaneImpl(execution_context->graphics,
                       ViewportImpl::From(viewport));
}

PlaneImpl::PlaneImpl(RenderScreenImpl* screen,
                     scoped_refptr<ViewportImpl> parent)
    : GraphicsChild(screen),
      Disposable(screen),
      node_(parent ? parent->GetDrawableController()
                   : screen->GetDrawableController(),
            SortKey()),
      viewport_(parent),
      src_rect_(new RectImpl(base::Rect())),
      scale_(1.0f),
      color_(new ColorImpl(base::Vec4())),
      tone_(new ToneImpl(base::Vec4())) {
  node_.RegisterEventHandler(base::BindRepeating(
      &PlaneImpl::DrawableNodeHandlerInternal, base::Unretained(this)));

  agent_ = new PlaneAgent;
  screen->PostTask(
      base::BindOnce(&GPUCreatePlaneInternal, screen->GetDevice(), agent_));
}

PlaneImpl::~PlaneImpl() {
  ExceptionState exception_state;
  Dispose(exception_state);
}

void PlaneImpl::SetLabel(const std::string& label,
                         ExceptionState& exception_state) {
  node_.SetDebugLabel(label);
}

void PlaneImpl::Dispose(ExceptionState& exception_state) {
  Disposable::Dispose(exception_state);
}

bool PlaneImpl::IsDisposed(ExceptionState& exception_state) {
  return Disposable::IsDisposed(exception_state);
}

scoped_refptr<Bitmap> PlaneImpl::Get_Bitmap(ExceptionState& exception_state) {
  return bitmap_;
}

void PlaneImpl::Put_Bitmap(const scoped_refptr<Bitmap>& value,
                           ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  bitmap_ = CanvasImpl::FromBitmap(value);
  if (bitmap_)
    src_rect_->SetBase(bitmap_->AsBaseSize());
}

scoped_refptr<Rect> PlaneImpl::Get_SrcRect(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return src_rect_;
}

void PlaneImpl::Put_SrcRect(const scoped_refptr<Rect>& value,
                            ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  *src_rect_ = *RectImpl::From(value);
}

scoped_refptr<Viewport> PlaneImpl::Get_Viewport(
    ExceptionState& exception_state) {
  return viewport_;
}

void PlaneImpl::Put_Viewport(const scoped_refptr<Viewport>& value,
                             ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  if (viewport_ == value)
    return;

  viewport_ = ViewportImpl::From(value);
  node_.RebindController(viewport_ ? viewport_->GetDrawableController()
                                   : screen()->GetDrawableController());
}

bool PlaneImpl::Get_Visible(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return false;

  return node_.GetVisibility();
}

void PlaneImpl::Put_Visible(const bool& value,
                            ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  node_.SetNodeVisibility(value);
}

int32_t PlaneImpl::Get_Z(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return node_.GetSortKeys()->weight[0];
}

void PlaneImpl::Put_Z(const int32_t& value, ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  node_.SetNodeSortWeight(value);
}

int32_t PlaneImpl::Get_Ox(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return origin_.x;
}

void PlaneImpl::Put_Ox(const int32_t& value, ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  origin_.x = value;
}

int32_t PlaneImpl::Get_Oy(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return origin_.y;
}

void PlaneImpl::Put_Oy(const int32_t& value, ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  origin_.y = value;
}

float PlaneImpl::Get_ZoomX(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return scale_.x;
}

void PlaneImpl::Put_ZoomX(const float& value, ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  scale_.x = value;
}

float PlaneImpl::Get_ZoomY(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return scale_.y;
}

void PlaneImpl::Put_ZoomY(const float& value, ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  scale_.y = value;
}

int32_t PlaneImpl::Get_Opacity(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return opacity_;
}

void PlaneImpl::Put_Opacity(const int32_t& value,
                            ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  opacity_ = std::clamp(value, 0, 255);
}

int32_t PlaneImpl::Get_BlendType(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return blend_type_;
}

void PlaneImpl::Put_BlendType(const int32_t& value,
                              ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  blend_type_ = value;
}

scoped_refptr<Color> PlaneImpl::Get_Color(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return color_;
}

void PlaneImpl::Put_Color(const scoped_refptr<Color>& value,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  *color_ = *ColorImpl::From(value);
}

scoped_refptr<Tone> PlaneImpl::Get_Tone(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return tone_;
}

void PlaneImpl::Put_Tone(const scoped_refptr<Tone>& value,
                         ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  *tone_ = *ToneImpl::From(value);
}

void PlaneImpl::OnObjectDisposed() {
  node_.DisposeNode();

  screen()->PostTask(base::BindOnce(&GPUDestroyPlaneInternal, agent_));
  agent_ = nullptr;
}

void PlaneImpl::DrawableNodeHandlerInternal(
    DrawableNode::RenderStage stage,
    DrawableNode::RenderControllerParams* params) {
  if (!bitmap_ || !bitmap_->GetAgent())
    return;

  if (stage == DrawableNode::RenderStage::BEFORE_RENDER) {
    screen()->PostTask(base::BindOnce(
        &GPUUpdatePlaneQuadArrayInternal, params->device, params->context,
        agent_, bitmap_->GetAgent(), src_rect_->AsBaseRect(),
        params->viewport.Size(), scale_, origin_ + params->origin, opacity_,
        color_->AsNormColor(), tone_->AsNormColor()));
  } else if (stage == DrawableNode::RenderStage::ON_RENDERING) {
    screen()->PostTask(base::BindOnce(
        &GPUOnViewportRenderingInternal, params->device, params->context,
        params->world_binding, agent_, bitmap_->GetAgent(), blend_type_));
  }
}

}  // namespace content
