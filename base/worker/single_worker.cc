// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/worker/single_worker.h"

#include "fiber/fiber.h"

#include "base/worker/isolate_scheduler.h"

namespace base {

SingleWorker::SingleWorker(WorkerScheduleMode mode)
    : scheduler_(nullptr), mode_(mode), task_queue_() {}

SingleWorker::~SingleWorker() {}

std::unique_ptr<SingleWorker> SingleWorker::CreateWorker(
    WorkerScheduleMode mode) {
  return std::unique_ptr<SingleWorker>(new SingleWorker(mode));
}

bool SingleWorker::SendTask(WorkerTaskTraits&& task) {
  return task_queue_.enqueue(std::move(task));
}

bool SingleWorker::WaitWorkerSynchronize() {
  moodycamel::LightweightSemaphore semaphore;
  WorkerTaskTraits required_task =
      base::BindOnce([](moodycamel::LightweightSemaphore* semaphore,
                        SingleWorker* worker) { semaphore->signal(); },
                     &semaphore);
  task_queue_.enqueue(std::move(required_task));
  return semaphore.tryWait();
}

void SingleWorker::Flush() {
  WorkerTaskTraits queued_task;

  if (mode_ == WorkerScheduleMode::kAsync)
    task_queue_.wait_dequeue_timed(queued_task, std::chrono::milliseconds(1));
  else
    task_queue_.try_dequeue(queued_task);

  if (!queued_task.is_null())
    std::move(queued_task).Run(this);
}

void SingleWorker::YieldFiber() {
  if (scheduler_)
    fiber_switch(scheduler_->primary_coroutine_);
}

}  // namespace base
