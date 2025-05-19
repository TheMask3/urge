// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/canvas/canvas_scheduler.h"

namespace content {

CanvasScheduler::~CanvasScheduler() = default;

std::unique_ptr<CanvasScheduler> CanvasScheduler::MakeInstance(
    base::ThreadWorker* worker,
    renderer::RenderDevice* device,
    filesystem::IOService* io_service) {
  return std::unique_ptr<CanvasScheduler>(
      new CanvasScheduler(worker, device, io_service));
}

renderer::RenderDevice* CanvasScheduler::GetDevice() const {
  return device_;
}

filesystem::IOService* CanvasScheduler::GetIOService() const {
  return io_service_;
}

void CanvasScheduler::SubmitPendingPaintCommands() {
  for (auto it = children_.head(); it != children_.end(); it = it->next()) {
    // Submit all pending commands (except Blt, StretchBlt)
    it->value()->SubmitQueuedCommands();
  }
}

void CanvasScheduler::SetupRenderTarget(Diligent::ITextureView* render_target,
                                        bool clear_target) {
  auto* context = device_->GetContext();

  // Clear cached state
  if (current_render_target_ != render_target)
    context->InvalidateState();
  current_render_target_ = render_target;

  // Setup new render target
  if (current_render_target_) {
    context->SetRenderTargets(
        1, &current_render_target_, nullptr,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    if (clear_target) {
      float clear_color[] = {0, 0, 0, 0};
      context->ClearRenderTarget(
          current_render_target_, clear_color,
          Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    return;
  }

  // Reset render target state
  context->SetRenderTargets(
      0, nullptr, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // Apply clear buffer if need
}

CanvasScheduler::CanvasScheduler(base::ThreadWorker* worker,
                                 renderer::RenderDevice* device,
                                 filesystem::IOService* io_service)
    : device_(device),
      render_worker_(worker),
      io_service_(io_service),
      current_render_target_(nullptr),
      generic_base_binding_(device->GetPipelines()->base.CreateBinding()),
      generic_color_binding_(device->GetPipelines()->color.CreateBinding()),
      common_quad_batch_(renderer::QuadBatch::Make(**device)) {}

}  // namespace content
