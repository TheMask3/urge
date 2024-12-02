// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RENDERER_CONTEXT_DEVICE_CONTEXT_H_
#define RENDERER_CONTEXT_DEVICE_CONTEXT_H_

#include <vector>

#include "renderer/device/render_device.h"
#include "renderer/vertex/vertex_layout.h"

namespace renderer {

class DeviceContext {
 public:
  ~DeviceContext();

  DeviceContext(const DeviceContext&) = delete;
  DeviceContext& operator=(const DeviceContext&) = delete;

  static std::unique_ptr<DeviceContext> MakeContextFor(RenderDevice* device);

  // Return available command encoder for immediately mode context.
  wgpu::CommandEncoder* GetImmediateEncoder();

  // Submit queued command in encoder to gpu,
  // flush current queue and create new queue.
  void Flush();

 private:
  DeviceContext(const wgpu::Device& device);

  wgpu::Device device_;
  wgpu::CommandEncoder immediate_encoder_;
};

}  // namespace renderer

#endif  //! RENDERER_CONTEXT_DEVICE_CONTEXT_H_
