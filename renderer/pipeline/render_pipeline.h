// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RENDERER_PIPELINE_RENDER_PIPELINE_H_
#define RENDERER_PIPELINE_RENDER_PIPELINE_H_

#include "renderer/device/render_device.h"

namespace renderer {

class RenderPipelineBase {
 public:
  ~RenderPipelineBase() = default;

  RenderPipelineBase(const RenderPipelineBase&) = delete;
  RenderPipelineBase& operator=(const RenderPipelineBase&) = delete;

  wgpu::RenderPipeline* GetPipeline() { return &pipeline_; }

 protected:
  RenderPipelineBase(const wgpu::Device& device);

  void BuildPipeline(
      const std::string& shader_source,
      const std::string& vs_entry,
      const std::string& fs_entry,
      const std::vector<wgpu::VertexBufferLayout>& vertex_layout,
      const std::vector<wgpu::BindGroupLayout>& bind_layout,
      const std::vector<wgpu::ColorTargetState>& attachment_target);

 private:
  wgpu::Device device_;
  wgpu::RenderPipeline pipeline_;
};

}  // namespace renderer

#endif  //! RENDERER_PIPELINE_RENDER_PIPELINE_H_
