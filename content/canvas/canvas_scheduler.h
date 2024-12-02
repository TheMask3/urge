// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CANVAS_CANVAS_SCHEDULER_H_
#define CONTENT_CANVAS_CANVAS_SCHEDULER_H_

#include "base/worker/single_worker.h"
#include "renderer/context/device_context.h"
#include "renderer/device/render_device.h"

namespace content {

class CanvasScheduler {
 public:
  ~CanvasScheduler();

  CanvasScheduler(const CanvasScheduler&) = delete;
  CanvasScheduler& operator=(const CanvasScheduler&) = delete;

  static std::unique_ptr<CanvasScheduler> MakeInstance(
      renderer::RenderDevice* device,
      renderer::DeviceContext* context);

  renderer::RenderDevice* GetDevice();
  renderer::DeviceContext* GetDrawContext();

  // Bind a worker for current scheduler,
  // all bitmap/canvas draw command will be encoded on this worker.
  // If worker set to null, it will be executed immediately on caller thread.
  void BindRenderWorker(base::SingleWorker* worker);

 private:
  CanvasScheduler(renderer::RenderDevice* device,
                  renderer::DeviceContext* context);

  renderer::RenderDevice* logic_device_;
  renderer::DeviceContext* painter_context_;
  base::SingleWorker* render_worker_;
};

}  // namespace content

#endif  //! CONTENT_CANVAS_CANVAS_SCHEDULER_H_
