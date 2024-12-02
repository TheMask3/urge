// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/rect_impl.h"

namespace content {

RectImpl::RectImpl(const base::Rect& rect) : rect_(rect), dirty_(true) {}

scoped_refptr<RectImpl> RectImpl::From(scoped_refptr<Rect> host) {
  return static_cast<RectImpl*>(host.get());
}

void RectImpl::Set(int32_t x, int32_t y, int32_t width, int32_t height) {
  rect_ = base::Rect(x, y, width, height);
  dirty_ = true;
}

void RectImpl::Set(scoped_refptr<Rect> other) {
  rect_ = static_cast<RectImpl*>(other.get())->rect_;
  dirty_ = true;
}

void RectImpl::Empty() {
  rect_ = base::Rect();
  dirty_ = true;
}

bool RectImpl::FetchDirtyStatus() {
  if (dirty_) {
    dirty_ = false;
    return true;
  } else
    return false;
}

base::Rect RectImpl::AsBaseRect() {
  base::Rect tmp(rect_);
  tmp.width = std::max(0, tmp.width);
  tmp.height = std::max(0, tmp.height);
  return tmp;
}

int32_t RectImpl::Get_X() {
  return rect_.x;
}

void RectImpl::Put_X(const int32_t& value) {
  if (rect_.x != value) {
    rect_.x = value;
    dirty_ = true;
  }
}

int32_t RectImpl::Get_Y() {
  return rect_.y;
}

void RectImpl::Put_Y(const int32_t& value) {
  if (rect_.y != value) {
    rect_.y = value;
    dirty_ = true;
  }
}

int32_t RectImpl::Get_Width() {
  return rect_.width;
}

void RectImpl::Put_Width(const int32_t& value) {
  if (rect_.width != value) {
    rect_.width = value;
    dirty_ = true;
  }
}

int32_t RectImpl::Get_Height() {
  return rect_.height;
}

void RectImpl::Put_Height(const int32_t& value) {
  if (rect_.height != value) {
    rect_.height = value;
    dirty_ = true;
  }
}

}  // namespace content
