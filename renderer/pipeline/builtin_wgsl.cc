// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "renderer/pipeline/builtin_wgsl.h"

namespace renderer {

const std::string kBaseRenderWGSL = R"(

struct VertexOutput {
  @builtin(position) pos: vec4<f32>,
  @location(0) uv: vec2<f32>,
  @location(1) color: vec4<f32>,
};

@group(0) @binding(0) var<uniform> u_transform: mat4x4<f32>;
@group(1) @binding(0) var<uniform> u_tex_size: vec2<f32>;
@group(1) @binding(1) var u_texture: texture_2d<f32>;
@group(1) @binding(2) var u_sampler: sampler;

@vertex fn vertexMain(
    @location(0) pos: vec4<f32>,
    @location(1) uv: vec2<f32>,
    @location(2) color: vec4<f32>) -> VertexOutput {
  var result: VertexOutput;
  result.pos = u_transform * pos;
  result.uv = uv * u_tex_size;
  result.color = color;
  return result;
}

@fragment fn fragmentMain(vertex: VertexOutput) -> @location(0) vec4f {
  var tex = textureSample(u_texture, u_sampler, vertex.uv);
  tex.a *= vertex.color.a;
  return tex;
}

)";

const std::string kColorRenderWGSL = R"(

struct VertexOutput {
  @builtin(position) pos: vec4<f32>,
  @location(0) color: vec4<f32>,
};

@group(0) @binding(0) var<uniform> u_transform: mat4x4<f32>;

@vertex fn vertexMain(
    @location(0) pos: vec4<f32>,
    @location(1) uv: vec2<f32>,
    @location(2) color: vec4<f32>) -> VertexOutput {
  var result: VertexOutput;
  result.pos = u_transform * pos;
  result.color = color;
  return result;
}

@fragment fn fragmentMain(vertex: VertexOutput) -> @location(0) vec4f {
  return vertex.color;
}

)";

}  // namespace renderer
