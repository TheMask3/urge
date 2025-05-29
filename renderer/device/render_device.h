// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RENDERER_DEVICE_RENDER_DEVICE_H_
#define RENDERER_DEVICE_RENDER_DEVICE_H_

#include "renderer/context/render_context.h"
#include "renderer/context/scissor_controller.h"
#include "renderer/pipeline/render_pipeline.h"
#include "renderer/resource/render_buffer.h"
#include "ui/widget/widget.h"

#include <memory>
#include <tuple>

namespace renderer {

enum class DriverType {
  UNDEFINED = 0,
  OPENGL,
  VULKAN,
  D3D11,
  D3D12,
};

class RenderDevice {
 public:
  struct PipelineSet {
    Pipeline_Base base;
    Pipeline_Color color;
    Pipeline_Flat viewport;
    Pipeline_Sprite sprite;
    Pipeline_AlphaTransition alphatrans;
    Pipeline_VagueTransition mappedtrans;
    Pipeline_Tilemap tilemap;
    Pipeline_Tilemap2 tilemap2;
    Pipeline_BitmapHue bitmaphue;
    Pipeline_Spine2D spine2d;
    Pipeline_YUV yuv;

    PipelineSet(Diligent::RefCntAutoPtr<Diligent::IRenderDevice> device,
                Diligent::TEXTURE_FORMAT target_format)
        : base(device, target_format),
          color(device, target_format),
          viewport(device, target_format),
          sprite(device, target_format),
          alphatrans(device, target_format),
          mappedtrans(device, target_format),
          tilemap(device, target_format),
          tilemap2(device, target_format),
          bitmaphue(device, target_format),
          spine2d(device, target_format),
          yuv(device, target_format) {}
  };

  using CreateDeviceResult =
      std::tuple<std::unique_ptr<RenderDevice>, std::unique_ptr<RenderContext>>;
  static CreateDeviceResult Create(base::WeakPtr<ui::Widget> window_target,
                                   DriverType driver_type);

  ~RenderDevice();

  RenderDevice(const RenderDevice&) = delete;
  RenderDevice& operator=(const RenderDevice&) = delete;

  // Device access
  inline Diligent::IRenderDevice* operator->() { return device_; }
  inline Diligent::IRenderDevice* operator*() { return device_; }

  // Device Attribute interface
  base::WeakPtr<ui::Widget> GetWindow() { return window_; }
  Diligent::ISwapChain* GetSwapChain() const { return swapchain_; }

  // Pre-compile shaders set storage
  PipelineSet* GetPipelines() const { return pipelines_.get(); }
  QuadIndexCache* GetQuadIndex() const { return quad_index_.get(); }

  // Platform specific type
  inline bool IsUVFlip() const {
    return device_type_ == Diligent::RENDER_DEVICE_TYPE_GL ||
           device_type_ == Diligent::RENDER_DEVICE_TYPE_GLES;
  }

  // Managed mobile rendering context
  void SuspendContext();
  int32_t ResumeContext(Diligent::IDeviceContext* immediate_context);

 private:
  RenderDevice(base::WeakPtr<ui::Widget> window,
               const Diligent::SwapChainDesc& swapchain_desc,
               Diligent::RefCntAutoPtr<Diligent::IRenderDevice> device,
               Diligent::RefCntAutoPtr<Diligent::ISwapChain> swapchain,
               std::unique_ptr<PipelineSet> pipelines,
               std::unique_ptr<QuadIndexCache> quad_index,
               SDL_GLContext gl_context);

  base::WeakPtr<ui::Widget> window_;
  Diligent::SwapChainDesc swapchain_desc_;

  Diligent::RefCntAutoPtr<Diligent::IRenderDevice> device_;
  Diligent::RefCntAutoPtr<Diligent::ISwapChain> swapchain_;

  std::unique_ptr<PipelineSet> pipelines_;
  std::unique_ptr<QuadIndexCache> quad_index_;

  Diligent::RENDER_DEVICE_TYPE device_type_;
  SDL_GLContext gl_context_;
};

}  // namespace renderer

#endif  //! RENDERER_DEVICE_RENDER_DEVICE_H_
