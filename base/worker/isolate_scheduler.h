// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WORKER_ISOLATE_SCHEDULER_H_
#define BASE_WORKER_ISOLATE_SCHEDULER_H_

#include <atomic>
#include <thread>
#include <unordered_map>

#include "fiber/fiber.h"

#include "base/worker/single_worker.h"

namespace base {

class WorkerScheduler {
 public:
  ~WorkerScheduler();

  WorkerScheduler(const WorkerScheduler&) = delete;
  WorkerScheduler& operator=(const WorkerScheduler&) = delete;

  static std::unique_ptr<WorkerScheduler> Create();

  // Add a worker for current scheduler,
  // rawptr can be reserved until scheduler destroyed.
  void AddChildWorker(std::unique_ptr<SingleWorker> child);

  // Flush scheduler working process, update coroutine mode workers' running.
  void Flush();

 private:
  friend class SingleWorker;
  friend class std::unique_ptr<WorkerScheduler>;
  WorkerScheduler(fiber_t* primary);

  static void FiberRunnerInternal(fiber_t* coroutine);
  void ThreadRunnerInternal(SingleWorker* worker);

  fiber_t* primary_coroutine_;
  std::unordered_map<std::unique_ptr<SingleWorker>, fiber_t*>
      coroutine_workers_;
  std::unordered_map<std::unique_ptr<SingleWorker>, std::thread> async_workers_;

  std::atomic<bool> quit_flag_;
};

}  // namespace base

#endif  //! BASE_WORKER_ISOLATE_SCHEDULER_H_
