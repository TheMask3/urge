// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SCREEN_RENDERSCREEN_IMPL_H_
#define CONTENT_SCREEN_RENDERSCREEN_IMPL_H_

#include "content/public/engine_graphics.h"
#include "content/render/drawable_controller.h"

namespace content {

class RenderScreenImpl : public Graphics {
 public:
  ~RenderScreenImpl() override;

  RenderScreenImpl(const RenderScreenImpl&) = delete;
  RenderScreenImpl& operator=(const RenderScreenImpl&) = delete;

 protected:
  void Update() override;
  void Wait(uint32_t duration) override;
  void FadeOut(uint32_t duration) override;
  void FadeIn(uint32_t duration) override;
  void Freeze() override;
  void Transition(uint32_t duration,
                  const std::string& filename,
                  uint32_t vague) override;
  scoped_refptr<Bitmap> SnapToBitmap() override;
  void FrameReset() override;
  uint32_t Width() override;
  uint32_t Height() override;
  void ResizeScreen(uint32_t width, uint32_t height) override;
  void PlayMovie(const std::string& filename) override;
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(FrameRate, uint32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(FrameCount, uint32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Brightness, uint32_t);

 private:
  DrawNodeController controller_;
};

}  // namespace content

#endif  //! CONTENT_SCREEN_RENDERSCREEN_IMPL_H_
