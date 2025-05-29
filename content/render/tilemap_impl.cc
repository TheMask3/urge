// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/render/tilemap_impl.h"

#include "renderer/utils/texture_utils.h"

namespace content {

namespace {

// Reference: RPGXP Editor - Autotile double click
// Format: Left Top -> Right Top -> Left Bottom -> Right Bottom
const base::Vec2 kAutotileSrcRegular[][4] = {
    {
        {1.0f, 2.0f},
        {1.5f, 2.0f},
        {1.0f, 2.5f},
        {1.5f, 2.5f},
    },
    {
        {2.0f, 0.0f},
        {1.5f, 2.0f},
        {1.0f, 2.5f},
        {1.5f, 2.5f},
    },
    {
        {1.0f, 2.0f},
        {2.5f, 0.0f},
        {1.0f, 2.5f},
        {1.5f, 2.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 0.0f},
        {1.0f, 2.5f},
        {1.5f, 2.5f},
    },
    {
        {1.0f, 2.0f},
        {1.5f, 2.0f},
        {1.0f, 2.5f},
        {2.5f, 0.5f},
    },
    {
        {2.0f, 0.0f},
        {1.5f, 2.0f},
        {1.0f, 2.5f},
        {2.5f, 0.5f},
    },
    {
        {1.0f, 2.0f},
        {2.5f, 0.0f},
        {1.0f, 2.5f},
        {2.5f, 0.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 0.0f},
        {1.0f, 2.5f},
        {2.5f, 0.5f},
    },
    {
        {1.0f, 2.0f},
        {1.5f, 2.0f},
        {2.0f, 0.5f},
        {1.5f, 2.5f},
    },
    {
        {2.0f, 0.0f},
        {1.5f, 2.0f},
        {2.0f, 0.5f},
        {1.5f, 2.5f},
    },
    {
        {1.0f, 2.0f},
        {2.5f, 0.0f},
        {2.0f, 0.5f},
        {1.5f, 2.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 0.0f},
        {2.0f, 0.5f},
        {1.5f, 2.5f},
    },
    {
        {1.0f, 2.0f},
        {1.5f, 2.0f},
        {2.0f, 0.5f},
        {2.5f, 0.5f},
    },
    {
        {2.0f, 0.0f},
        {1.5f, 2.0f},
        {2.0f, 0.5f},
        {2.5f, 0.5f},
    },
    {
        {1.0f, 2.0f},
        {2.5f, 0.0f},
        {2.0f, 0.5f},
        {2.5f, 0.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 0.0f},
        {2.0f, 0.5f},
        {2.5f, 0.5f},
    },
    {
        {0.0f, 2.0f},
        {0.5f, 2.0f},
        {0.0f, 2.5f},
        {0.5f, 2.5f},
    },
    {
        {0.0f, 2.0f},
        {2.5f, 0.0f},
        {0.0f, 2.5f},
        {0.5f, 2.5f},
    },
    {
        {0.0f, 2.0f},
        {0.5f, 2.0f},
        {0.0f, 2.5f},
        {2.5f, 0.5f},
    },
    {
        {0.0f, 2.0f},
        {2.5f, 0.0f},
        {0.0f, 2.5f},
        {2.5f, 0.5f},
    },
    {
        {1.0f, 1.0f},
        {1.5f, 1.0f},
        {1.0f, 1.5f},
        {1.5f, 1.5f},
    },
    {
        {1.0f, 1.0f},
        {1.5f, 1.0f},
        {1.0f, 1.5f},
        {2.5f, 0.5f},
    },
    {
        {1.0f, 1.0f},
        {1.5f, 1.0f},
        {2.0f, 0.5f},
        {1.5f, 1.5f},
    },
    {
        {1.0f, 1.0f},
        {1.5f, 1.0f},
        {2.0f, 0.5f},
        {2.5f, 0.5f},
    },
    {
        {2.0f, 2.0f},
        {2.5f, 2.0f},
        {2.0f, 2.5f},
        {2.5f, 2.5f},
    },
    {
        {2.0f, 2.0f},
        {2.5f, 2.0f},
        {2.0f, 0.5f},
        {2.5f, 2.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 2.0f},
        {2.0f, 2.5f},
        {2.5f, 2.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 2.0f},
        {2.0f, 0.5f},
        {2.5f, 2.5f},
    },
    {
        {1.0f, 3.0f},
        {1.5f, 3.0f},
        {1.0f, 3.5f},
        {1.5f, 3.5f},
    },
    {
        {2.0f, 0.0f},
        {1.5f, 3.0f},
        {1.0f, 3.5f},
        {1.5f, 3.5f},
    },
    {
        {1.0f, 3.0f},
        {2.5f, 0.0f},
        {1.0f, 3.5f},
        {1.5f, 3.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 0.0f},
        {1.0f, 3.5f},
        {1.5f, 3.5f},
    },
    {
        {0.0f, 2.0f},
        {2.5f, 2.0f},
        {0.0f, 2.5f},
        {2.5f, 2.5f},
    },
    {
        {1.0f, 1.0f},
        {1.5f, 1.0f},
        {1.0f, 3.5f},
        {1.5f, 3.5f},
    },
    {
        {0.0f, 1.0f},
        {0.5f, 1.0f},
        {0.0f, 1.5f},
        {0.5f, 1.5f},
    },
    {
        {0.0f, 1.0f},
        {0.5f, 1.0f},
        {0.0f, 1.5f},
        {2.5f, 0.5f},
    },
    {
        {2.0f, 1.0f},
        {2.5f, 1.0f},
        {2.0f, 1.5f},
        {2.5f, 1.5f},
    },
    {
        {2.0f, 1.0f},
        {2.5f, 1.0f},
        {2.0f, 0.5f},
        {2.5f, 1.5f},
    },
    {
        {2.0f, 3.0f},
        {2.5f, 3.0f},
        {2.0f, 3.5f},
        {2.5f, 3.5f},
    },
    {
        {2.0f, 0.0f},
        {2.5f, 3.0f},
        {2.0f, 3.5f},
        {2.5f, 3.5f},
    },
    {
        {0.0f, 3.0f},
        {0.5f, 3.0f},
        {0.0f, 3.5f},
        {0.5f, 3.5f},
    },
    {
        {0.0f, 3.0f},
        {2.5f, 0.0f},
        {0.0f, 3.5f},
        {0.5f, 3.5f},
    },
    {
        {0.0f, 1.0f},
        {2.5f, 1.0f},
        {0.0f, 1.5f},
        {2.5f, 1.5f},
    },
    {
        {0.0f, 1.0f},
        {0.5f, 1.0f},
        {0.0f, 3.5f},
        {0.5f, 3.5f},
    },
    {
        {0.0f, 3.0f},
        {2.5f, 3.0f},
        {0.0f, 3.5f},
        {2.5f, 3.5f},
    },
    {
        {2.0f, 1.0f},
        {2.5f, 1.0f},
        {2.0f, 3.5f},
        {2.5f, 3.5f},
    },
    {
        {0.0f, 1.0f},
        {2.5f, 1.0f},
        {0.0f, 3.5f},
        {2.5f, 3.5f},
    },
    {
        {0.0f, 0.0f},
        {0.5f, 0.0f},
        {0.0f, 0.5f},
        {0.5f, 0.5f},
    },
};

void GPUCreateTilemapInternal(renderer::RenderDevice* device,
                              TilemapAgent* agent) {
  agent->batch = renderer::QuadBatch::Make(**device);
  agent->shader_binding = device->GetPipelines()->tilemap.CreateBinding();

  Diligent::CreateUniformBuffer(
      **device, sizeof(renderer::Binding_Tilemap::Params), "tilemap.uniform",
      &agent->uniform_buffer, Diligent::USAGE_DEFAULT,
      Diligent::BIND_UNIFORM_BUFFER, Diligent::CPU_ACCESS_NONE);
}

void GPUDestroyTilemapInternal(TilemapAgent* agent) {
  delete agent;
}

void GPUMakeAtlasInternal(
    renderer::RenderDevice* device,
    renderer::RenderContext* context,
    TilemapAgent* agent,
    const base::Vec2i& atlas_size,
    std::vector<TilemapImpl::AtlasCompositeCommand> make_commands) {
  renderer::CreateTexture2D(**device, &agent->atlas_texture, "tilemap.atlas",
                            atlas_size);
  agent->atlas_binding = agent->atlas_texture->GetDefaultView(
      Diligent::TEXTURE_VIEW_SHADER_RESOURCE);

  for (auto& it : make_commands) {
    Diligent::CopyTextureAttribs copy_attribs;
    Diligent::Box box;

    copy_attribs.pSrcTexture = it.texture->data;
    copy_attribs.pSrcBox = &box;
    copy_attribs.SrcTextureTransitionMode =
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

    box.MinX = it.src_rect.x;
    box.MinY = it.src_rect.y;
    box.MaxX = it.src_rect.x + it.src_rect.width;
    box.MaxY = it.src_rect.y + it.src_rect.height;

    if (it.src_rect.x + it.src_rect.width > it.texture->size.x ||
        it.src_rect.y + it.src_rect.height > it.texture->size.y)
      continue;

    copy_attribs.pDstTexture = agent->atlas_texture;
    copy_attribs.DstX = it.dst_pos.x;
    copy_attribs.DstY = it.dst_pos.y;
    copy_attribs.DstTextureTransitionMode =
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

    if (it.dst_pos.x + it.src_rect.width > atlas_size.x ||
        it.dst_pos.y + it.src_rect.height > atlas_size.y)
      continue;

    (*context)->CopyTexture(copy_attribs);
  }
}

void GPUUploadTilesBatchInternal(
    renderer::RenderDevice* device,
    renderer::RenderContext* context,
    TilemapAgent* agent,
    std::vector<renderer::Quad> ground_cache,
    std::vector<std::vector<renderer::Quad>> aboves_cache) {
  std::vector<renderer::Quad> total_quads;
  agent->ground_draw_count = ground_cache.size();

  int32_t offset = ground_cache.size();
  agent->above_draw_count.clear();
  for (const auto& it : aboves_cache) {
    agent->above_draw_count.push_back(it.size());
    offset += it.size();
  }

  // Make total buffer
  total_quads.resize(offset);
  std::memcpy(total_quads.data(), ground_cache.data(),
              ground_cache.size() * sizeof(renderer::Quad));
  int32_t buffer_offset = ground_cache.size();
  for (const auto& it : aboves_cache) {
    std::memcpy(total_quads.data() + buffer_offset, it.data(),
                it.size() * sizeof(renderer::Quad));
    buffer_offset += it.size();
  }

  if (!total_quads.empty()) {
    // Upload data
    agent->batch->QueueWrite(**context, total_quads.data(), total_quads.size());

    // Allocate quad index
    device->GetQuadIndex()->Allocate(offset);
  }
}

void GPUUpdateTilemapUniformInternal(renderer::RenderDevice* device,
                                     renderer::RenderContext* context,
                                     TilemapAgent* agent,
                                     const base::Vec2& offset,
                                     int32_t tilesize,
                                     int32_t anim_index) {
  base::Vec2i atlas_size(agent->atlas_texture->GetDesc().Width,
                         agent->atlas_texture->GetDesc().Height);

  renderer::Binding_Tilemap::Params uniform;
  uniform.OffsetAndTexSize =
      base::MakeVec4(offset, base::MakeInvert(atlas_size));
  uniform.AnimateIndexAndTileSize.x = anim_index / 16;
  uniform.AnimateIndexAndTileSize.y = tilesize;

  (*context)->UpdateBuffer(agent->uniform_buffer, 0, sizeof(uniform), &uniform,
                           Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void GPURenderGroundLayerInternal(renderer::RenderDevice* device,
                                  renderer::RenderContext* context,
                                  Diligent::IBuffer** world_binding,
                                  TilemapAgent* agent) {
  if (!agent->atlas_texture)
    return;

  if (agent->ground_draw_count) {
    auto& pipeline_set = device->GetPipelines()->tilemap;
    auto* pipeline = pipeline_set.GetPipeline(renderer::BlendType::NORMAL);

    // Setup uniform params
    agent->shader_binding->u_transform->Set(*world_binding);
    agent->shader_binding->u_texture->Set(agent->atlas_binding);
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
    draw_indexed_attribs.NumIndices = 6 * agent->ground_draw_count;
    draw_indexed_attribs.IndexType = renderer::QuadIndexCache::kValueType;
    (*context)->DrawIndexed(draw_indexed_attribs);
  }
}

void GPURenderAboveLayerInternal(renderer::RenderDevice* device,
                                 renderer::RenderContext* context,
                                 Diligent::IBuffer** world_binding,
                                 TilemapAgent* agent,
                                 int32_t index) {
  if (!agent->atlas_texture)
    return;

  if (!agent->above_draw_count.empty()) {
    if (int32_t draw_count = agent->above_draw_count[index]) {
      int32_t draw_offset = agent->ground_draw_count;
      for (int32_t i = 0; i < index; ++i)
        draw_offset += agent->above_draw_count[i];

      auto& pipeline_set = device->GetPipelines()->tilemap;
      auto* pipeline = pipeline_set.GetPipeline(renderer::BlendType::NORMAL);

      // Setup uniform params
      agent->shader_binding->u_transform->Set(*world_binding);
      agent->shader_binding->u_texture->Set(agent->atlas_binding);
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
      draw_indexed_attribs.NumIndices = 6 * draw_count;
      draw_indexed_attribs.IndexType = renderer::QuadIndexCache::kValueType;
      draw_indexed_attribs.FirstIndexLocation = draw_offset * 6;
      (*context)->DrawIndexed(draw_indexed_attribs);
    }
  }
}

}  // namespace

TilemapAutotileImpl::TilemapAutotileImpl(base::WeakPtr<TilemapImpl> tilemap)
    : tilemap_(tilemap) {}

TilemapAutotileImpl::~TilemapAutotileImpl() = default;

scoped_refptr<Bitmap> TilemapAutotileImpl::Get(
    int32_t index,
    ExceptionState& exception_state) {
  if (!tilemap_)
    return nullptr;

  auto& autotiles = tilemap_->autotiles_;
  if (index < 0 || index >= static_cast<int32_t>(autotiles.size())) {
    exception_state.ThrowError(ExceptionCode::CONTENT_ERROR,
                               "Out range of autotiles.");
    return nullptr;
  }

  return autotiles[index].bitmap;
}

void TilemapAutotileImpl::Put(int32_t index,
                              scoped_refptr<Bitmap> texture,
                              ExceptionState& exception_state) {
  if (!tilemap_)
    return;

  auto& autotiles = tilemap_->autotiles_;
  if (index < 0 || index >= static_cast<int32_t>(autotiles.size()))
    return exception_state.ThrowError(ExceptionCode::CONTENT_ERROR,
                                      "Out range of autotiles.");

  autotiles[index].bitmap = CanvasImpl::FromBitmap(texture);
  autotiles[index].observer = autotiles[index].bitmap->AddCanvasObserver(
      base::BindRepeating(&TilemapImpl::AtlasModifyHandlerInternal, tilemap_));
  tilemap_->atlas_dirty_ = true;
}

scoped_refptr<Tilemap> Tilemap::New(ExecutionContext* execution_context,
                                    scoped_refptr<Viewport> viewport,
                                    int32_t tilesize,
                                    ExceptionState& exception_state) {
  return new TilemapImpl(execution_context->graphics,
                         ViewportImpl::From(viewport), std::max(16, tilesize));
}

TilemapImpl::TilemapImpl(RenderScreenImpl* screen,
                         scoped_refptr<ViewportImpl> parent,
                         int32_t tilesize)
    : GraphicsChild(screen),
      Disposable(screen),
      ground_node_(parent ? parent->GetDrawableController()
                          : screen->GetDrawableController(),
                   SortKey()),
      tilesize_(tilesize),
      viewport_(parent) {
  ground_node_.RegisterEventHandler(base::BindRepeating(
      &TilemapImpl::GroundNodeHandlerInternal, base::Unretained(this)));

  agent_ = new TilemapAgent;
  screen->PostTask(
      base::BindOnce(&GPUCreateTilemapInternal, screen->GetDevice(), agent_));
}

TilemapImpl::~TilemapImpl() {
  ExceptionState exception_state;
  Dispose(exception_state);
}

void TilemapImpl::SetLabel(const std::string& label,
                           ExceptionState& exception_state) {
  ground_node_.SetDebugLabel(label);
}

void TilemapImpl::Dispose(ExceptionState& exception_state) {
  Disposable::Dispose(exception_state);
}

bool TilemapImpl::IsDisposed(ExceptionState& exception_state) {
  return Disposable::IsDisposed(exception_state);
}

void TilemapImpl::Update(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  anim_index_ = ++anim_index_ % 64;

  flash_timer_ = ++flash_timer_ % 32;
  flash_opacity_ = std::abs(16 - flash_timer_) * 8 + 32;
  if (flash_count_)
    map_buffer_dirty_ = true;
}

scoped_refptr<TilemapAutotile> TilemapImpl::Autotiles(
    ExceptionState& exception_state) {
  return new TilemapAutotileImpl(weak_ptr_factory_.GetWeakPtr());
}

scoped_refptr<Viewport> TilemapImpl::Get_Viewport(
    ExceptionState& exception_state) {
  return viewport_;
}

void TilemapImpl::Put_Viewport(const scoped_refptr<Viewport>& value,
                               ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  if (viewport_ == value)
    return;

  viewport_ = ViewportImpl::From(value);
  DrawNodeController* controller = viewport_
                                       ? viewport_->GetDrawableController()
                                       : screen()->GetDrawableController();
  ground_node_.RebindController(controller);
  for (auto& it : above_nodes_)
    it.RebindController(controller);
}

scoped_refptr<Bitmap> TilemapImpl::Get_Tileset(
    ExceptionState& exception_state) {
  return tileset_;
}

void TilemapImpl::Put_Tileset(const scoped_refptr<Bitmap>& value,
                              ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  if (tileset_ == value)
    return;

  tileset_ = CanvasImpl::FromBitmap(value);
  tileset_observer_ = tileset_->AddCanvasObserver(base::BindRepeating(
      &TilemapImpl::AtlasModifyHandlerInternal, base::Unretained(this)));
  atlas_dirty_ = true;
}

scoped_refptr<Table> TilemapImpl::Get_MapData(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return map_data_;
}

void TilemapImpl::Put_MapData(const scoped_refptr<Table>& value,
                              ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  map_data_ = TableImpl::From(value);
  map_data_observer_ = map_data_->AddObserver(base::BindRepeating(
      &TilemapImpl::MapDataModifyHandlerInternal, base::Unretained(this)));
  map_buffer_dirty_ = true;
}

scoped_refptr<Table> TilemapImpl::Get_FlashData(
    ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return flash_data_;
}

void TilemapImpl::Put_FlashData(const scoped_refptr<Table>& value,
                                ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  flash_data_ = TableImpl::From(value);
  flash_data_observer_ = map_data_->AddObserver(base::BindRepeating(
      &TilemapImpl::MapDataModifyHandlerInternal, base::Unretained(this)));
  map_buffer_dirty_ = true;
}

scoped_refptr<Table> TilemapImpl::Get_Priorities(
    ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return priorities_;
}

void TilemapImpl::Put_Priorities(const scoped_refptr<Table>& value,
                                 ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  priorities_ = TableImpl::From(value);
  priorities_observer_ = priorities_->AddObserver(base::BindRepeating(
      &TilemapImpl::MapDataModifyHandlerInternal, base::Unretained(this)));
  map_buffer_dirty_ = true;
}

bool TilemapImpl::Get_Visible(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return ground_node_.GetVisibility();
}

void TilemapImpl::Put_Visible(const bool& value,
                              ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  ground_node_.SetNodeVisibility(value);
  for (auto& it : above_nodes_)
    it.SetNodeVisibility(value);
}

int32_t TilemapImpl::Get_Ox(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return origin_.x;
}

void TilemapImpl::Put_Ox(const int32_t& value,
                         ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  origin_.x = value;
  map_buffer_dirty_ = true;
}

int32_t TilemapImpl::Get_Oy(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return origin_.y;
}

void TilemapImpl::Put_Oy(const int32_t& value,
                         ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  origin_.y = value;
  map_buffer_dirty_ = true;
}

void TilemapImpl::OnObjectDisposed() {
  ground_node_.DisposeNode();
  above_nodes_.clear();

  screen()->PostTask(base::BindOnce(&GPUDestroyTilemapInternal, agent_));
  agent_ = nullptr;
}

void TilemapImpl::GroundNodeHandlerInternal(
    DrawableNode::RenderStage stage,
    DrawableNode::RenderControllerParams* params) {
  if (stage == DrawableNode::RenderStage::BEFORE_RENDER) {
    // Setup above layers if need.
    if (!(last_viewport_ == params->viewport)) {
      SetupTilemapLayersInternal(params->viewport);
      last_viewport_ = params->viewport;
    }

    // Reset above layers sorting order.
    UpdateViewportInternal(params->viewport, params->origin);
    ResetAboveLayersOrderInternal();

    // Generate global atlas texture if need.
    if (atlas_dirty_) {
      std::vector<AtlasCompositeCommand> commands;
      base::Vec2i size = MakeAtlasInternal(commands);

      screen()->PostTask(base::BindOnce(&GPUMakeAtlasInternal, params->device,
                                        params->context, agent_, size,
                                        std::move(commands)));
      atlas_dirty_ = false;
    }

    // Parse and generate map buffer if need.
    if (map_buffer_dirty_) {
      std::vector<renderer::Quad> ground_cache;
      std::vector<std::vector<renderer::Quad>> aboves_cache;
      ParseMapDataInternal(ground_cache, aboves_cache);

      screen()->PostTask(base::BindOnce(
          &GPUUploadTilesBatchInternal, params->device, params->context, agent_,
          std::move(ground_cache), std::move(aboves_cache)));
      map_buffer_dirty_ = false;
    }

    // Update tilemap uniform per frame.
    screen()->PostTask(base::BindOnce(&GPUUpdateTilemapUniformInternal,
                                      params->device, params->context, agent_,
                                      render_offset_, tilesize_, anim_index_));
  } else if (stage == DrawableNode::RenderStage::ON_RENDERING) {
    screen()->PostTask(base::BindOnce(&GPURenderGroundLayerInternal,
                                      params->device, params->context,
                                      params->world_binding, agent_));
  }
}

void TilemapImpl::AboveNodeHandlerInternal(
    int32_t layer_index,
    DrawableNode::RenderStage stage,
    DrawableNode::RenderControllerParams* params) {
  if (stage == DrawableNode::RenderStage::ON_RENDERING) {
    screen()->PostTask(base::BindOnce(
        &GPURenderAboveLayerInternal, params->device, params->context,
        params->world_binding, agent_, layer_index));
  }
}

base::Vec2i TilemapImpl::MakeAtlasInternal(
    std::vector<AtlasCompositeCommand>& commands) {
  int32_t atlas_height = 28 * tilesize_;
  if (tileset_ || tileset_->GetAgent())
    atlas_height = std::max(atlas_height, tileset_->AsBaseSize().y);

  // Setup commands
  commands.clear();

  // Autotile part
  int32_t offset = 0;
  for (auto& it : autotiles_) {
    if (!it.bitmap || !it.bitmap->GetAgent()) {
      ++offset;
      continue;
    }

    auto autotile_size = it.bitmap->AsBaseSize();
    base::Vec2i dst_pos(0, offset * tilesize_ * 4);

    if (autotile_size.x > 3 * tilesize_ && autotile_size.y > tilesize_) {
      // Animated autotile
      it.type = AutotileType::ANIMATED;
    } else if (autotile_size.x <= 3 * tilesize_ &&
               autotile_size.y > tilesize_) {
      // Static autotile
      dst_pos.x += tilesize_ * 3;
      it.type = AutotileType::STATIC;
    } else if (autotile_size.x <= 4 * tilesize_ &&
               autotile_size.y <= tilesize_) {
      // Single animated tile
      it.type = AutotileType::SINGLE_ANIMATED;

      for (int32_t i = 0; i < 4; ++i) {
        base::Rect single_src(base::Vec2i(tilesize_ * i, 0),
                              base::Vec2i(tilesize_));
        base::Vec2i single_dst(base::Vec2i(tilesize_ * i * 3, 0));

        commands.push_back({it.bitmap->GetAgent(), single_src, single_dst});
      }

      ++offset;
      continue;
    } else {
      NOTREACHED();
    }

    commands.push_back({it.bitmap->GetAgent(), autotile_size, dst_pos});

    ++offset;
  }

  // Tileset part
  if (tileset_ && tileset_->GetAgent()) {
    auto tileset_size = tileset_->AsBaseSize();
    base::Vec2i dst_pos(12 * tilesize_, 0);

    commands.push_back({tileset_->GetAgent(), tileset_size, dst_pos});
  }

  return base::Vec2i(tilesize_ * 20, atlas_height);
}

void TilemapImpl::UpdateViewportInternal(const base::Rect& viewport,
                                         const base::Vec2i& viewport_origin) {
  const base::Vec2i tilemap_origin = origin_ + viewport_origin;
  const base::Vec2i viewport_size = viewport.Size();

  base::Rect new_viewport;
  new_viewport.x = tilemap_origin.x / tilesize_;
  new_viewport.y = tilemap_origin.y / tilesize_ - 1;
  new_viewport.width =
      (viewport_size.x / tilesize_) + !!(viewport_size.x % tilesize_) + 1;
  new_viewport.height =
      (viewport_size.y / tilesize_) + !!(viewport_size.y % tilesize_) + 2;

  const base::Vec2i display_offset = tilemap_origin % base::Vec2i(tilesize_);
  render_offset_ = -display_offset - base::Vec2i(0, tilesize_);

  if (!(new_viewport == render_viewport_)) {
    render_viewport_ = new_viewport;
    map_buffer_dirty_ = true;
  }
}

void TilemapImpl::ParseMapDataInternal(
    std::vector<renderer::Quad>& ground_cache,
    std::vector<std::vector<renderer::Quad>>& aboves_cache) {
  auto set_autotile_pos = [&](base::RectF& pos, int32_t index) {
    switch (index) {
      case 0:  // Left Top
        break;
      case 1:  // Right Top
        pos.x += tilesize_ / 2.0f;
        break;
      case 2:  // Left Bottom
        pos.y += tilesize_ / 2.0f;
        break;
      case 3:  // Right Bottom
        pos.x += tilesize_ / 2.0f;
        pos.y += tilesize_ / 2.0f;
        break;
      default:
        break;
    }
  };

  auto get_priority = [&](int16_t tile_id) -> int32_t {
    if (!priorities_ || tile_id >= static_cast<int32_t>(priorities_->x_size()))
      return 0;

    int16_t value = priorities_->value(tile_id, 0, 0);
    if (value > 5)
      return -1;

    return value;
  };

  auto process_autotile = [&](const base::Vec2i& pos, int16_t tile_id,
                              const base::Vec4& color,
                              std::vector<renderer::Quad>* target) {
    // Autotile (0-7)
    int32_t autotile_id = tile_id / 48 - 1;
    // Pattern (0-47)
    int32_t pattern_id = tile_id % 48;

    // Autotile invalid check
    AutotileInfo& info = autotiles_[autotile_id];
    if (!info.bitmap || !info.bitmap->GetAgent())
      return;

    // Generate from autotile type
    switch (info.type) {
      case AutotileType::ANIMATED:
      case AutotileType::STATIC: {
        const base::Vec2* autotile_src_pos = kAutotileSrcRegular[pattern_id];
        for (int32_t i = 0; i < 4; ++i) {
          base::RectF tex_src(autotile_src_pos[i], base::Vec2(0.5f));
          tex_src.x = tex_src.x * tilesize_ + 0.5f;
          tex_src.y = tex_src.y * tilesize_ + 0.5f;
          tex_src.width = tex_src.width * tilesize_ - 1.0f;
          tex_src.height = tex_src.height * tilesize_ - 1.0f;

          if (info.type == AutotileType::STATIC)
            tex_src.x += 3 * tilesize_;
          tex_src.y += autotile_id * 4 * tilesize_;

          base::RectF chunk_pos(pos.x * tilesize_, pos.y * tilesize_,
                                tilesize_ / 2.0f, tilesize_ / 2.0f);
          set_autotile_pos(chunk_pos, i);

          renderer::Quad quad;
          renderer::Quad::SetPositionRect(&quad, chunk_pos);
          renderer::Quad::SetTexCoordRectNorm(&quad, tex_src);
          renderer::Quad::SetColor(&quad, color);
          target->push_back(quad);
        }
      } break;
      case AutotileType::SINGLE_ANIMATED: {
        const base::RectF single_tex(0.5f,
                                     0.5f + autotile_id * tilesize_ * 4.0f,
                                     tilesize_ - 1.0f, tilesize_ - 1.0f);
        const base::RectF single_pos(pos.x * tilesize_, pos.y * tilesize_,
                                     tilesize_, tilesize_);

        renderer::Quad quad;
        renderer::Quad::SetPositionRect(&quad, single_pos);
        renderer::Quad::SetTexCoordRectNorm(&quad, single_tex);
        renderer::Quad::SetColor(&quad, color);
        target->push_back(quad);
      } break;
      default:
        NOTREACHED();
        break;
    }
  };

  auto value_wrap = [&](int32_t value, int32_t range) {
    int32_t res = value % range;
    return res < 0 ? res + range : res;
  };

  auto get_wrap_data = [&](scoped_refptr<TableImpl> t, int32_t x, int32_t y,
                           int32_t z) -> int16_t {
    if (!t)
      return 0;

    return t->value(value_wrap(x, t->x_size()), value_wrap(y, t->y_size()), z);
  };

  auto process_tile = [&](const base::Vec2i& pos, int32_t z) {
    int32_t tile_id = get_wrap_data(map_data_, pos.x + render_viewport_.x,
                                    pos.y + render_viewport_.y, z);
    int32_t flash_color = get_wrap_data(flash_data_, pos.x + render_viewport_.x,
                                        pos.y + render_viewport_.y, 0);

    if (tile_id < 48)
      return;

    int32_t priority = get_priority(tile_id);
    if (priority == -1)
      return;

    std::vector<renderer::Quad>* target;
    if (!priority) {
      // Ground layer
      target = &ground_cache;
    } else {
      // Above multi layers
      target = &aboves_cache[pos.y + priority];
    }

    // Composite flash color
    base::Vec4 blend_color;
    if (flash_color) {
      const float blue = ((flash_color & 0x000F) >> 0) / 0xF;
      const float green = ((flash_color & 0x00F0) >> 4) / 0xF;
      const float red = ((flash_color & 0x0F00) >> 8) / 0xF;
      blend_color = base::Vec4(red, green, blue, flash_opacity_ / 255.0f);

      ++flash_count_;
    }

    if (tile_id < 48 * 8)
      return process_autotile(pos, tile_id, blend_color, target);

    int32_t tileset_id = tile_id - 48 * 8;
    int32_t tile_x = tileset_id % 8;
    int32_t tile_y = tileset_id / 8;

    base::Vec2 atlas_offset(12 + tile_x, tile_y);
    base::RectF quad_tex(atlas_offset.x * tilesize_ + 0.5f,
                         atlas_offset.y * tilesize_ + 0.5f, tilesize_ - 1.0f,
                         tilesize_ - 1.0f);
    base::RectF quad_pos(pos.x * tilesize_, pos.y * tilesize_, tilesize_,
                         tilesize_);

    renderer::Quad quad;
    renderer::Quad::SetPositionRect(&quad, quad_pos);
    renderer::Quad::SetTexCoordRectNorm(&quad, quad_tex);
    renderer::Quad::SetColor(&quad, blend_color);
    target->push_back(quad);
  };

  auto process_buffer = [&]() {
    for (int32_t x = 0; x < render_viewport_.width; ++x)
      for (int32_t y = 0; y < render_viewport_.height; ++y)
        for (int32_t z = 0; z < static_cast<int32_t>(map_data_->z_size()); ++z)
          process_tile(base::Vec2i(x, y), z);
  };

  // Process map buffer
  int32_t above_layers_count = render_viewport_.height + 5;
  aboves_cache.resize(above_layers_count);

  // Reset flash quads
  flash_count_ = 0;

  // Begin to parse buffer data
  process_buffer();
}

void TilemapImpl::SetupTilemapLayersInternal(const base::Rect& viewport) {
  ground_node_.SetNodeSortWeight(0);

  above_nodes_.clear();
  int32_t above_layers_count =
      (viewport.height / tilesize_) + !!(viewport.height % tilesize_) + 7;
  for (int32_t i = 0; i < above_layers_count; ++i) {
    DrawableNode above_node(ground_node_.GetController(), SortKey());
    above_node.RegisterEventHandler(base::BindRepeating(
        &TilemapImpl::AboveNodeHandlerInternal, base::Unretained(this), i));
    above_nodes_.push_back(std::move(above_node));
  }
}

void TilemapImpl::ResetAboveLayersOrderInternal() {
  for (int32_t i = 0; i < static_cast<int32_t>(above_nodes_.size()); ++i) {
    int32_t layer_order = 32 * (i + render_viewport_.y + 1) - origin_.y;
    above_nodes_[i].SetNodeSortWeight(layer_order);
  }
}

void TilemapImpl::AtlasModifyHandlerInternal() {
  atlas_dirty_ = true;
}

void TilemapImpl::MapDataModifyHandlerInternal() {
  map_buffer_dirty_ = true;
}

}  // namespace content
