// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "renderer/pipeline/render_pipeline.h"

#include "base/debug/logging.h"
#include "renderer/pipeline/builtin_hlsl.h"

namespace renderer {

namespace {

static constexpr char GAMMA_TO_LINEAR[] =
    "((Gamma) < 0.04045 ? (Gamma) / 12.92 : pow(max((Gamma) + 0.055, 0.0) / "
    "1.055, 2.4))";

static constexpr char SRGBA_TO_LINEAR[] =
    "col.r = GAMMA_TO_LINEAR(col.r); "
    "col.g = GAMMA_TO_LINEAR(col.g); "
    "col.b = GAMMA_TO_LINEAR(col.b); "
    "col.a = 1.0 - GAMMA_TO_LINEAR(1.0 - col.a);";

Diligent::RenderTargetBlendDesc GetBlendState(BlendType type) {
  Diligent::RenderTargetBlendDesc state;
  switch (type) {
    default:
    case BlendType::NORMAL:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_SRC_ALPHA;
      state.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      break;
    case BlendType::ADDITION:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_SRC_ALPHA;
      state.DestBlend = Diligent::BLEND_FACTOR_ONE;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      break;
    case BlendType::SUBSTRACTION:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_REV_SUBTRACT;
      state.SrcBlend = Diligent::BLEND_FACTOR_SRC_ALPHA;
      state.DestBlend = Diligent::BLEND_FACTOR_ONE;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_ZERO;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      break;
    case BlendType::MULTIPLY:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_DEST_COLOR;
      state.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      break;
    case BlendType::SCREEN:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_ONE;
      state.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      break;
    case BlendType::KEEP_ALPHA:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_SRC_ALPHA;
      state.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_ZERO;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      break;
    case BlendType::NORMAL_PMA:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_ONE;
      state.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
      break;
    case BlendType::ADDITION_PMA:
      state.BlendEnable = Diligent::True;
      state.BlendOp = Diligent::BLEND_OPERATION_ADD;
      state.SrcBlend = Diligent::BLEND_FACTOR_ONE;
      state.DestBlend = Diligent::BLEND_FACTOR_ONE;
      state.SrcBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      state.DestBlendAlpha = Diligent::BLEND_FACTOR_ONE;
      break;
    case BlendType::NO_BLEND:
      state.BlendEnable = Diligent::False;
      break;
  }

  return state;
}

}  // namespace

RenderPipelineBase::RenderPipelineBase(Diligent::IRenderDevice* device)
    : device_(device) {}

void RenderPipelineBase::BuildPipeline(
    const ShaderSource& shader_source,
    const std::vector<Diligent::LayoutElement>& input_layout,
    const std::vector<
        Diligent::RefCntAutoPtr<Diligent::IPipelineResourceSignature>>&
        signatures,
    Diligent::TEXTURE_FORMAT target_format) {
  // Make pipeline debug name
  std::stringstream pipeline_debug_name;
  pipeline_debug_name << "pipeline<" << shader_source.name << ">";
  std::string pipeline_name = pipeline_debug_name.str();

  // Make graphics pipeline state
  Diligent::GraphicsPipelineStateCreateInfo pipeline_state_desc;
  pipeline_state_desc.PSODesc.Name = pipeline_name.c_str();
  pipeline_state_desc.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;

  pipeline_state_desc.GraphicsPipeline.NumRenderTargets = 1;
  pipeline_state_desc.GraphicsPipeline.RTVFormats[0] = target_format;
  pipeline_state_desc.GraphicsPipeline.PrimitiveTopology =
      Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pipeline_state_desc.GraphicsPipeline.RasterizerDesc.CullMode =
      Diligent::CULL_MODE_NONE;
  pipeline_state_desc.GraphicsPipeline.RasterizerDesc.ScissorEnable =
      Diligent::True;
  pipeline_state_desc.GraphicsPipeline.DepthStencilDesc.DepthEnable =
      Diligent::False;

  // Make vertex shader and pixel shader
  Diligent::ShaderCreateInfo shader_desc;
  Diligent::RefCntAutoPtr<Diligent::IShader> vertex_shader_object,
      pixel_shader_object;
  shader_desc.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
  shader_desc.Desc.UseCombinedTextureSamplers = Diligent::True;
  shader_desc.CompileFlags =
      Diligent::SHADER_COMPILE_FLAG_PACK_MATRIX_ROW_MAJOR;

  {
    // Vertex shader
    shader_desc.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
    shader_desc.EntryPoint = shader_source.vertex_entry.c_str();
    shader_desc.Desc.Name = shader_source.name.c_str();
    shader_desc.Source = shader_source.source.c_str();
    shader_desc.SourceLength = shader_source.source.size();
    shader_desc.Macros.Count = shader_source.macros.size();
    shader_desc.Macros.Elements = shader_source.macros.data();
    device_->CreateShader(shader_desc, &vertex_shader_object);

    // Pixel shader
    shader_desc.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
    shader_desc.EntryPoint = shader_source.pixel_entry.c_str();
    shader_desc.Desc.Name = shader_source.name.c_str();
    shader_desc.Source = shader_source.source.c_str();
    shader_desc.SourceLength = shader_source.source.size();
    shader_desc.Macros.Count = shader_source.macros.size();
    shader_desc.Macros.Elements = shader_source.macros.data();
    device_->CreateShader(shader_desc, &pixel_shader_object);
  }

  pipeline_state_desc.pVS = vertex_shader_object;
  pipeline_state_desc.pPS = pixel_shader_object;

  // Setup input attribute elements
  pipeline_state_desc.GraphicsPipeline.InputLayout.LayoutElements =
      input_layout.data();
  pipeline_state_desc.GraphicsPipeline.InputLayout.NumElements =
      input_layout.size();

  // Setup resource signature
  resource_signatures_ = signatures;
  std::vector<Diligent::IPipelineResourceSignature*> raw_signatures;
  for (const auto& it : resource_signatures_)
    raw_signatures.push_back(it);
  pipeline_state_desc.ResourceSignaturesCount = resource_signatures_.size();
  pipeline_state_desc.ppResourceSignatures = raw_signatures.data();

  // Make all color blend type pipelines
  for (int32_t i = 0; i < BlendType::TYPE_NUMS; ++i) {
    pipeline_state_desc.GraphicsPipeline.BlendDesc.RenderTargets[0] =
        GetBlendState(static_cast<BlendType>(i));

    Diligent::RefCntAutoPtr<Diligent::IPipelineState> pipeline_state;
    device_->CreateGraphicsPipelineState(pipeline_state_desc, &pipeline_state);

    pipelines_.push_back(pipeline_state);
  }
}

Diligent::RefCntAutoPtr<Diligent::IPipelineResourceSignature>
RenderPipelineBase::MakeResourceSignature(
    const std::vector<Diligent::PipelineResourceDesc>& variables,
    const std::vector<Diligent::ImmutableSamplerDesc>& samplers,
    uint8_t binding_index) {
  Diligent::PipelineResourceSignatureDesc resource_signature_desc;
  resource_signature_desc.Resources = variables.data();
  resource_signature_desc.NumResources = variables.size();
  resource_signature_desc.ImmutableSamplers = samplers.data();
  resource_signature_desc.NumImmutableSamplers = samplers.size();
  resource_signature_desc.BindingIndex = binding_index;
  resource_signature_desc.UseCombinedTextureSamplers = Diligent::True;

  Diligent::RefCntAutoPtr<Diligent::IPipelineResourceSignature> signature;
  device_->CreatePipelineResourceSignature(resource_signature_desc, &signature);
  return signature;
}

Pipeline_Base::Pipeline_Base(Diligent::IRenderDevice* device,
                             Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_BaseRender, "base.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_Texture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Color::Pipeline_Color(Diligent::IRenderDevice* device,
                               Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_ColorRender, "color.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  auto binding0 = MakeResourceSignature(variables, {}, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Flat::Pipeline_Flat(Diligent::IRenderDevice* device,
                             Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_FlatRender, "flat.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "FlatUniformConstants",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_Texture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Sprite::Pipeline_Sprite(Diligent::IRenderDevice* device,
                                 Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const auto& device_info = device->GetDeviceInfo();
  storage_buffer_support =
      !(device_info.Type == Diligent::RENDER_DEVICE_TYPE_GLES);

  if (!storage_buffer_support)
    LOG(INFO) << "[Pipeline] Disable Sprite batch process.";

  Diligent::ShaderMacro vertex_macro = {"STORAGE_BUFFER_SUPPORT",
                                        storage_buffer_support ? "1" : "0"};

  const ShaderSource shader_source{
      kHLSL_SpriteRender, "sprite.render", {vertex_macro}};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_VERTEX,
       storage_buffer_support ? "u_Params" : "SpriteUniformParam",
       storage_buffer_support ? Diligent::SHADER_RESOURCE_TYPE_BUFFER_SRV
                              : Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_Texture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_AlphaTransition::Pipeline_AlphaTransition(
    Diligent::IRenderDevice* device,
    Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_AlphaTransitionRender,
                                   "alpha.trans.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_PIXEL, "u_FrozenTexture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_CurrentTexture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_FrozenTexture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_CurrentTexture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_VagueTransition::Pipeline_VagueTransition(
    Diligent::IRenderDevice* device,
    Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_MappingTransitionRender,
                                   "vague.trans.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_PIXEL, "u_FrozenTexture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_CurrentTexture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_TransTexture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_FrozenTexture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_CurrentTexture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_TransTexture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Tilemap::Pipeline_Tilemap(Diligent::IRenderDevice* device,
                                   Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_TilemapRender, "tilemap.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_VERTEX, "TilemapUniformBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_Texture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Tilemap2::Pipeline_Tilemap2(Diligent::IRenderDevice* device,
                                     Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_Tilemap2Render, "tilemap2.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_VERTEX, "Tilemap2UniformBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_Texture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_BitmapHue::Pipeline_BitmapHue(Diligent::IRenderDevice* device,
                                       Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_BitmapHueRender, "hue.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_Texture",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Spine2D::Pipeline_Spine2D(Diligent::IRenderDevice* device,
                                   Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_Spine2DRender, "spine2d.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture_sampler",
       Diligent::SHADER_RESOURCE_TYPE_SAMPLER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  auto binding0 = MakeResourceSignature(variables, {}, 0);
  BuildPipeline(shader_source, SpineVertex::GetLayout(), {binding0},
                target_format);
}

Pipeline_YUV::Pipeline_YUV(Diligent::IRenderDevice* device,
                           Diligent::TEXTURE_FORMAT target_format)
    : RenderPipelineBase(device) {
  const ShaderSource shader_source{kHLSL_YUVRender, "yuv.render"};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_PIXEL, "u_TextureY",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_TextureU",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_TextureV",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  const std::vector<Diligent::ImmutableSamplerDesc> samplers = {
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_TextureY",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_TextureU",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
      {
          Diligent::SHADER_TYPE_PIXEL,
          "u_TextureV",
          {Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_POINT,
           Diligent::FILTER_TYPE_POINT, Diligent::TEXTURE_ADDRESS_CLAMP,
           Diligent::TEXTURE_ADDRESS_CLAMP, Diligent::TEXTURE_ADDRESS_CLAMP},
      },
  };

  auto binding0 = MakeResourceSignature(variables, samplers, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

Pipeline_Present::Pipeline_Present(Diligent::IRenderDevice* device,
                                   Diligent::TEXTURE_FORMAT target_format,
                                   bool manual_srgb)
    : RenderPipelineBase(device) {
  std::vector<Diligent::ShaderMacro> pixel_macros = {
      {"GAMMA_TO_LINEAR(Gamma)", GAMMA_TO_LINEAR},
      {"SRGBA_TO_LINEAR(col)", manual_srgb ? SRGBA_TO_LINEAR : ""},
  };

  const ShaderSource shader_source{kHLSL_PresentRender, "present.render",
                                   pixel_macros};

  const std::vector<Diligent::PipelineResourceDesc> variables = {
      {Diligent::SHADER_TYPE_VERTEX, "WorldMatrixBuffer",
       Diligent::SHADER_RESOURCE_TYPE_CONSTANT_BUFFER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture",
       Diligent::SHADER_RESOURCE_TYPE_TEXTURE_SRV,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      {Diligent::SHADER_TYPE_PIXEL, "u_Texture_sampler",
       Diligent::SHADER_RESOURCE_TYPE_SAMPLER,
       Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
  };

  auto binding0 = MakeResourceSignature(variables, {}, 0);
  BuildPipeline(shader_source, Vertex::GetLayout(), {binding0}, target_format);
}

}  // namespace renderer
