// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/canvas/canvas_scheduler.h"

namespace content {

CanvasScheduler::~CanvasScheduler() {}

std::unique_ptr<CanvasScheduler> CanvasScheduler::MakeInstance(
    renderer::RenderDevice* device,
    renderer::DeviceContext* context) {
  return std::unique_ptr<CanvasScheduler>(new CanvasScheduler(device, context));
}

renderer::RenderDevice* CanvasScheduler::GetDevice() {
  return logic_device_;
}

renderer::DeviceContext* CanvasScheduler::GetDrawContext() {
  return painter_context_;
}

void CanvasScheduler::BindRenderWorker(base::SingleWorker* worker) {
  render_worker_ = worker;
}

void CanvasScheduler::AttachChildCanvas(CanvasImpl* child) {
  children_.Append(child);
}

void CanvasScheduler::SubmitPendingPaintCommands(
    const wgpu::CommandEncoder& encoder) {
  for (auto it = children_.head(); it != children_.end(); it = it->next()) {
    // Submit all pending commands (except Blt, StretchBlt)
    it->value()->SubmitQueuedCommands(encoder);
  }
}

CanvasScheduler::CanvasScheduler(renderer::RenderDevice* device,
                                 renderer::DeviceContext* context)
    : logic_device_(device),
      painter_context_(context),
      render_worker_(nullptr) {}

}  // namespace content
