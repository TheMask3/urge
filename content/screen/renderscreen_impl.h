// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SCREEN_RENDERSCREEN_IMPL_H_
#define CONTENT_SCREEN_RENDERSCREEN_IMPL_H_

#include "content/public/engine_graphics.h"
#include "content/render/drawable_controller.h"
#include "renderer/device/render_device.h"

namespace content {

class RenderScreenImpl : public Graphics {
 public:
  RenderScreenImpl(std::unique_ptr<renderer::RenderDevice> device);
  ~RenderScreenImpl() override;

  RenderScreenImpl(const RenderScreenImpl&) = delete;
  RenderScreenImpl& operator=(const RenderScreenImpl&) = delete;

  renderer::RenderDevice* GetDevice() const;

 protected:
  void Update(ExceptionState& exception_state) override;
  void Wait(uint32_t duration, ExceptionState& exception_state) override;
  void FadeOut(uint32_t duration, ExceptionState& exception_state) override;
  void FadeIn(uint32_t duration, ExceptionState& exception_state) override;
  void Freeze(ExceptionState& exception_state) override;
  void Transition(uint32_t duration,
                  const std::string& filename,
                  uint32_t vague,
                  ExceptionState& exception_state) override;
  scoped_refptr<Bitmap> SnapToBitmap(ExceptionState& exception_state) override;
  void FrameReset(ExceptionState& exception_state) override;
  uint32_t Width(ExceptionState& exception_state) override;
  uint32_t Height(ExceptionState& exception_state) override;
  void ResizeScreen(uint32_t width,
                    uint32_t height,
                    ExceptionState& exception_state) override;
  void PlayMovie(const std::string& filename,
                 ExceptionState& exception_state) override;
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(FrameRate, uint32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(FrameCount, uint32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Brightness, uint32_t);

 private:
  DrawNodeController controller_;

  std::unique_ptr<renderer::RenderDevice> device_;

  base::Vec2i resolution_;
  wgpu::Texture screen_buffer_;
  wgpu::Texture frozen_buffer_;

  int32_t brightness_;
  uint64_t frame_count_;
};

}  // namespace content

#endif  //! CONTENT_SCREEN_RENDERSCREEN_IMPL_H_
