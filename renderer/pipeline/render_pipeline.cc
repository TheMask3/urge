// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "renderer/pipeline/render_pipeline.h"

namespace renderer {

RenderPipelineBase::RenderPipelineBase(const wgpu::Device& device)
    : device_(device) {}

void RenderPipelineBase::BuildPipeline(
    const std::string& shader_source,
    const std::string& vs_entry,
    const std::string& fs_entry,
    const std::vector<wgpu::VertexBufferLayout>& vertex_layout,
    const std::vector<wgpu::BindGroupLayout>& bind_layout,
    const std::vector<wgpu::ColorTargetState>& attachment_target) {
  wgpu::ShaderModuleWGSLDescriptor wgsl_desc;
  wgsl_desc.code = std::string_view(shader_source);

  wgpu::ShaderModuleDescriptor shader_module_desc;
  shader_module_desc.nextInChain = &wgsl_desc;
  wgpu::ShaderModule shader_module =
      device_.CreateShaderModule(&shader_module_desc);

  wgpu::PipelineLayoutDescriptor pipeline_layout_desc;
  pipeline_layout_desc.bindGroupLayoutCount = bind_layout.size();
  pipeline_layout_desc.bindGroupLayouts = bind_layout.data();
  wgpu::PipelineLayout pipeline_layout =
      device_.CreatePipelineLayout(&pipeline_layout_desc);

  wgpu::VertexState vertex_state;
  vertex_state.module = shader_module;
  vertex_state.entryPoint = std::string_view(vs_entry);
  vertex_state.bufferCount = vertex_layout.size();
  vertex_state.buffers = vertex_layout.data();

  wgpu::FragmentState fragment_state;
  fragment_state.module = shader_module;
  fragment_state.entryPoint = std::string_view(fs_entry);
  fragment_state.targetCount = attachment_target.size();
  fragment_state.targets = attachment_target.data();

  wgpu::RenderPipelineDescriptor descriptor;
  descriptor.layout = pipeline_layout;
  descriptor.vertex = vertex_state;
  descriptor.fragment = &fragment_state;

  pipeline_ = device_.CreateRenderPipeline(&descriptor);
}

}  // namespace renderer
