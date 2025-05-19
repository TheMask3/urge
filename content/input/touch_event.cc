// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/input/touch_event.h"

namespace content {

std::vector<scoped_refptr<TouchEvent>> TouchEvent::Update(
    ExecutionContext* execution_context,
    ExceptionState& exception_state) {
  std::vector<EventController::TouchEventData> raw_events;
  execution_context->event_controller->PollTouchEvents(raw_events);

  std::vector<scoped_refptr<TouchEvent>> filtered_events;
  for (auto& it : raw_events)
    filtered_events.push_back(new TouchEventImpl(it));

  return filtered_events;
}

TouchEventImpl::TouchEventImpl(EventController::TouchEventData event)
    : event_(event) {}

TouchEventImpl::~TouchEventImpl() = default;

TouchEvent::Type TouchEventImpl::GetType(ExceptionState& exception_state) {
  return event_.type;
}

int32_t TouchEventImpl::GetDeviceID(ExceptionState& exception_state) {
  return event_.touch_id;
}

int32_t TouchEventImpl::GetFinger(ExceptionState& exception_state) {
  return event_.finger;
}

int32_t TouchEventImpl::GetX(ExceptionState& exception_state) {
  return event_.x;
}

int32_t TouchEventImpl::GetY(ExceptionState& exception_state) {
  return event_.y;
}

int32_t TouchEventImpl::GetDeltaX(ExceptionState& exception_state) {
  return event_.dx;
}

int32_t TouchEventImpl::GetDeltaY(ExceptionState& exception_state) {
  return event_.dy;
}

float TouchEventImpl::GetPressure(ExceptionState& exception_state) {
  return event_.pressure;
}

}  // namespace content
