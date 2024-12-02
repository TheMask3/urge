// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RENDERER_RESOURCE_BUFFER_RECYCLE_H_
#define RENDERER_RESOURCE_BUFFER_RECYCLE_H_

#include "renderer/device/render_device.h"
#include "renderer/renderer_config.h"

namespace renderer {

class QuadrangleIndexCache {
 public:
  ~QuadrangleIndexCache() = default;

  static std::unique_ptr<QuadrangleIndexCache> Make(RenderDevice* device);

  // Allocate a new capacity index buffer for drawcall using,
  // |quadrangle_size| is the count not the byte size.
  wgpu::Buffer* Allocate(uint32_t quadrangle_size);

 private:
  QuadrangleIndexCache(const wgpu::Device& device);

  wgpu::Device device_;
  std::vector<uint16_t> cached_indices_;
  wgpu::Buffer index_buffer_;
  uint32_t count_;
};

template <typename VertexType>
class VertexBufferController {
 public:
  ~VertexBufferController() = default;

  VertexBufferController(const VertexBufferController&) = delete;
  VertexBufferController& operator=(const VertexBufferController&) = delete;

  static std::unique_ptr<VertexBufferController> Make(RenderDevice* device);

  // Manage current vertex buffer.
  void PushVertices(const VertexType* data, uint32_t size);

  // Upload vertex data to GPU
  void SubmitBuffer(const wgpu::CommandEncoder& encoder);

  // Clear pending vertex cache.
  void ClearPendingCache();

 private:
  VertexBufferController(const wgpu::Device& device);

  wgpu::Device device_;
  std::vector<VertexType> buffer_cache_;
  wgpu::Buffer vertex_buffer_;
};

template <typename VertexType>
inline VertexBufferController<VertexType>::VertexBufferController(
    const wgpu::Device& device)
    : device_(device), buffer_cache_() {}

template <typename VertexType>
inline std::unique_ptr<VertexBufferController<VertexType>>
VertexBufferController<VertexType>::Make(RenderDevice* device) {
  return std::unique_ptr<VertexBufferController>(
      new VertexBufferController<VertexType>(*device->GetDevice()));
}

template <typename VertexType>
inline void VertexBufferController<VertexType>::PushVertices(
    const VertexType* data,
    uint32_t size) {
  buffer_cache_.resize(buffer_cache_.size() + size);
  std::memcpy(buffer_cache_.data(), data, size * sizeof(VertexType));
}

template <typename VertexType>
inline void VertexBufferController<VertexType>::SubmitBuffer(
    const wgpu::CommandEncoder& encoder) {
  if (buffer_cache_.size())
    encoder.WriteBuffer(vertex_buffer_, 0,
                        reinterpret_cast<uint8_t*>(buffer_cache_.data()),
                        buffer_cache_.size() * sizeof(VertexType));
}

template <typename VertexType>
inline void VertexBufferController<VertexType>::ClearPendingCache() {
  buffer_cache_.clear();
}

}  // namespace renderer

#endif  //! RENDERER_RESOURCE_BUFFER_RECYCLE_H_
