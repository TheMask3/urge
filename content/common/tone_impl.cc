// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/tone_impl.h"

#include <algorithm>

namespace content {

scoped_refptr<Tone> Tone::New() {
  return new ToneImpl(base::Vec4(0));
}

scoped_refptr<Tone> Tone::New(float red, float green, float blue, float gray) {
  return new ToneImpl(base::Vec4(red, green, blue, gray));
}

ToneImpl::ToneImpl(const base::Vec4& value) : value_(value), dirty_(true) {
  value_.x = std::clamp(value_.x, -255.0f, 255.0f);
  value_.y = std::clamp(value_.y, -255.0f, 255.0f);
  value_.z = std::clamp(value_.z, -255.0f, 255.0f);
  value_.w = std::clamp(value_.w, 0.0f, 255.0f);
}

scoped_refptr<ToneImpl> ToneImpl::From(scoped_refptr<Tone> host) {
  return static_cast<ToneImpl*>(host.get());
}

void ToneImpl::Set(float red, float green, float blue, float gray) {
  value_.x = std::clamp(red, -255.0f, 255.0f);
  value_.y = std::clamp(green, -255.0f, 255.0f);
  value_.z = std::clamp(blue, -255.0f, 255.0f);
  value_.w = std::clamp(gray, 0.0f, 255.0f);
  dirty_ = true;
}

void ToneImpl::Set(scoped_refptr<Tone> other) {
  value_ = static_cast<ToneImpl*>(other.get())->value_;
  dirty_ = true;
}

std::pair<bool, base::Vec4> ToneImpl::FetchUpdateRequiredAndData() {
  bool has_changed = dirty_;

  if (dirty_) {
    dirty_ = false;

    norm_.x = value_.x / 255.0f;
    norm_.y = value_.y / 255.0f;
    norm_.z = value_.z / 255.0f;
    norm_.w = value_.w / 255.0f;
  }

  return std::make_pair(has_changed, norm_);
}

float ToneImpl::Get_Red() {
  return value_.x;
}

void ToneImpl::Put_Red(const float& value) {
  if (value_.x != value) {
    dirty_ = true;
    value_.x = std::clamp(value, -255.0f, 255.0f);
  }
}

float ToneImpl::Get_Green() {
  return value_.y;
}

void ToneImpl::Put_Green(const float& value) {
  if (value_.y != value) {
    dirty_ = true;
    value_.y = std::clamp(value, -255.0f, 255.0f);
  }
}

float ToneImpl::Get_Blue() {
  return value_.z;
}

void ToneImpl::Put_Blue(const float& value) {
  if (value_.z != value) {
    dirty_ = true;
    value_.z = std::clamp(value, -255.0f, 255.0f);
  }
}

float ToneImpl::Get_Gray() {
  return value_.w;
}

void ToneImpl::Put_Gray(const float& value) {
  if (value_.w != value) {
    dirty_ = true;
    value_.w = std::clamp(value, 0.0f, 255.0f);
  }
}

}  // namespace content
