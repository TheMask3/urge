// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/screen/renderscreen_impl.h"

namespace content {

RenderScreenImpl::RenderScreenImpl(
    std::unique_ptr<renderer::RenderDevice> device) {}

RenderScreenImpl::~RenderScreenImpl() {}

renderer::RenderDevice* RenderScreenImpl::GetDevice() const {
  return device_.get();
}

void RenderScreenImpl::Update(ExceptionState& exception_state) {}

void RenderScreenImpl::Wait(uint32_t duration,
                            ExceptionState& exception_state) {}

void RenderScreenImpl::FadeOut(uint32_t duration,
                               ExceptionState& exception_state) {}

void RenderScreenImpl::FadeIn(uint32_t duration,
                              ExceptionState& exception_state) {}

void RenderScreenImpl::Freeze(ExceptionState& exception_state) {}

void RenderScreenImpl::Transition(uint32_t duration,
                                  const std::string& filename,
                                  uint32_t vague,
                                  ExceptionState& exception_state) {}

scoped_refptr<Bitmap> RenderScreenImpl::SnapToBitmap(
    ExceptionState& exception_state) {
  return scoped_refptr<Bitmap>();
}

void RenderScreenImpl::FrameReset(ExceptionState& exception_state) {}

uint32_t RenderScreenImpl::Width(ExceptionState& exception_state) {
  return 0;
}

uint32_t RenderScreenImpl::Height(ExceptionState& exception_state) {
  return 0;
}

void RenderScreenImpl::ResizeScreen(uint32_t width,
                                    uint32_t height,
                                    ExceptionState& exception_state) {}

void RenderScreenImpl::PlayMovie(const std::string& filename,
                                 ExceptionState& exception_state) {}

uint32_t RenderScreenImpl::Get_FrameRate(ExceptionState&) {}

void RenderScreenImpl::Put_FrameRate(const uint32_t&, ExceptionState&) {}

uint32_t RenderScreenImpl::Get_FrameCount(ExceptionState&) {}

void RenderScreenImpl::Put_FrameCount(const uint32_t&, ExceptionState&) {}

uint32_t RenderScreenImpl::Get_Brightness(ExceptionState&) {}

void RenderScreenImpl::Put_Brightness(const uint32_t&, ExceptionState&) {}

}  // namespace content
