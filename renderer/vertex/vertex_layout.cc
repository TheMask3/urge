// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "renderer/vertex/vertex_layout.h"

namespace renderer {

wgpu::VertexBufferLayout FullVertexLayout::GetLayout() {
  static wgpu::VertexAttribute attrs[3];

  // Position: vec4<f32>
  attrs[0].format = wgpu::VertexFormat::Float32x4;
  attrs[0].offset = 0 * sizeof(float);
  attrs[0].shaderLocation = 0;

  // TexCoord: vec2<f32>
  attrs[1].format = wgpu::VertexFormat::Float32x2;
  attrs[1].offset = 4 * sizeof(float);
  attrs[1].shaderLocation = 1;

  // Color: vec4<f32>
  attrs[2].format = wgpu::VertexFormat::Float32x4;
  attrs[2].offset = 6 * sizeof(float);
  attrs[2].shaderLocation = 2;

  wgpu::VertexBufferLayout layout;
  layout.arrayStride = sizeof(FullVertexLayout);
  layout.attributeCount = _countof(attrs);
  layout.attributes = attrs;
  layout.stepMode = wgpu::VertexStepMode::Vertex;

  return layout;
}

}  // namespace renderer
