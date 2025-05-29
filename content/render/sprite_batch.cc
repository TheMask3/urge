// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/render/sprite_batch.h"

namespace content {

SpriteBatch::SpriteBatch(renderer::RenderDevice* device,
                         std::unique_ptr<renderer::Binding_Sprite> binding,
                         std::unique_ptr<renderer::QuadBatch> vertex_batch,
                         std::unique_ptr<SpriteBatchBuffer> uniform_batch)
    : device_(device),
      current_texture_(nullptr),
      last_batch_index_(-1),
      binding_(std::move(binding)),
      vertex_batch_(std::move(vertex_batch)),
      uniform_batch_(std::move(uniform_batch)) {}

SpriteBatch::~SpriteBatch() {}

std::unique_ptr<SpriteBatch> SpriteBatch::Make(renderer::RenderDevice* device) {
  auto binding = device->GetPipelines()->sprite.CreateBinding();
  auto vertex_batch = renderer::QuadBatch::Make(**device);
  auto uniform_batch = SpriteBatchBuffer::Make(**device);

  return std::unique_ptr<SpriteBatch>(
      new SpriteBatch(device, std::move(binding), std::move(vertex_batch),
                      std::move(uniform_batch)));
}

void SpriteBatch::BeginBatch(TextureAgent* texture) {
  current_texture_ = texture;
  last_batch_index_ = uniform_cache_.size();
}

void SpriteBatch::PushSprite(const renderer::Quad& quad,
                             const renderer::Binding_Sprite::Params& uniform) {
  quad_cache_.push_back(quad);
  uniform_cache_.push_back(uniform);
}

void SpriteBatch::EndBatch(uint32_t* instance_offset,
                           uint32_t* instance_count) {
  const int32_t draw_count = uniform_cache_.size() - last_batch_index_;

  *instance_offset = last_batch_index_;
  *instance_count = draw_count;

  current_texture_ = nullptr;
  last_batch_index_ = -1;
}

void SpriteBatch::SubmitBatchDataAndResetCache(
    renderer::RenderDevice* device,
    renderer::RenderContext* context) {
  // Setup index buffer
  device_->GetQuadIndex()->Allocate(uniform_cache_.size());

  // Upload data and rebuild binding
  if (quad_cache_.size())
    vertex_batch_->QueueWrite(**context, quad_cache_.data(),
                              quad_cache_.size());

  if (uniform_cache_.size()) {
    uniform_batch_->QueueWrite(**context, uniform_cache_.data(),
                               uniform_cache_.size());
    uniform_binding_ =
        (**uniform_batch_)
            ->GetDefaultView(Diligent::BUFFER_VIEW_SHADER_RESOURCE);
  }

  // Reset cache
  quad_cache_.clear();
  uniform_cache_.clear();
}

}  // namespace content
