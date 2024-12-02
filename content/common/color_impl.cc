// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/color_impl.h"

#include <algorithm>

namespace content {

scoped_refptr<Color> Color::New() {
  return new ColorImpl(base::Vec4(0));
}

scoped_refptr<Color> Color::New(float red,
                                float green,
                                float blue,
                                float gray) {
  return new ColorImpl(base::Vec4(red, green, blue, gray));
}

ColorImpl::ColorImpl(const base::Vec4& value) : value_(value), dirty_(true) {
  value_.x = std::clamp(value_.x, 0.0f, 255.0f);
  value_.y = std::clamp(value_.y, 0.0f, 255.0f);
  value_.z = std::clamp(value_.z, 0.0f, 255.0f);
  value_.w = std::clamp(value_.w, 0.0f, 255.0f);
}

scoped_refptr<ColorImpl> ColorImpl::From(scoped_refptr<Color> host) {
  return static_cast<ColorImpl*>(host.get());
}

void ColorImpl::Set(float red, float green, float blue, float gray) {
  value_.x = std::clamp(red, 0.0f, 255.0f);
  value_.y = std::clamp(green, 0.0f, 255.0f);
  value_.z = std::clamp(blue, 0.0f, 255.0f);
  value_.w = std::clamp(gray, 0.0f, 255.0f);
  dirty_ = true;
}

void ColorImpl::Set(scoped_refptr<Color> other) {
  value_ = static_cast<ColorImpl*>(other.get())->value_;
  dirty_ = true;
}

SDL_Color ColorImpl::AsSDLColor() {
  return {static_cast<uint8_t>(value_.x), static_cast<uint8_t>(value_.y),
          static_cast<uint8_t>(value_.z), static_cast<uint8_t>(value_.w)};
}

std::pair<bool, base::Vec4> ColorImpl::FetchUpdateRequiredAndData() {
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

float ColorImpl::Get_Red() {
  return value_.x;
}

void ColorImpl::Put_Red(const float& value) {
  if (value_.x != value) {
    dirty_ = true;
    value_.x = std::clamp(value, 0.0f, 255.0f);
  }
}

float ColorImpl::Get_Green() {
  return value_.y;
}

void ColorImpl::Put_Green(const float& value) {
  if (value_.y != value) {
    dirty_ = true;
    value_.y = std::clamp(value, 0.0f, 255.0f);
  }
}

float ColorImpl::Get_Blue() {
  return value_.z;
}

void ColorImpl::Put_Blue(const float& value) {
  if (value_.z != value) {
    dirty_ = true;
    value_.z = std::clamp(value, 0.0f, 255.0f);
  }
}

float ColorImpl::Get_Alpha() {
  return value_.w;
}

void ColorImpl::Put_Alpha(const float& value) {
  if (value_.w != value) {
    dirty_ = true;
    value_.w = std::clamp(value, 0.0f, 255.0f);
  }
}

}  // namespace content
