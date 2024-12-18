// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/color_impl.h"

#include <algorithm>

namespace content {

scoped_refptr<Color> Color::New(ExceptionState& exception_state) {
  return new ColorImpl(base::Vec4(0));
}

scoped_refptr<Color> Color::New(float red,
                                float green,
                                float blue,
                                float gray,
                                ExceptionState& exception_state) {
  return new ColorImpl(base::Vec4(red, green, blue, gray));
}

scoped_refptr<Color> Color::Copy(scoped_refptr<Color> other) {
  return new ColorImpl(*static_cast<ColorImpl*>(other.get()));
}

scoped_refptr<Color> Color::Deserialize(const std::string& data,
                                        ExceptionState& exception_state) {
  const float* ptr = reinterpret_cast<const float*>(data.data());
  ColorImpl* impl = new ColorImpl(base::Vec4(*ptr++, *ptr++, *ptr++, *ptr++));
  return impl;
}

std::string Color::Serialize(scoped_refptr<Color> value,
                             ExceptionState& exception_state) {
  ColorImpl* impl = static_cast<ColorImpl*>(value.get());
  std::string serial_data(sizeof(float) * 4, 0);
  memcpy(serial_data.data(), &impl->value_, sizeof(base::Vec4));
  return serial_data;
}

ColorImpl::ColorImpl(const base::Vec4& value) : value_(value), dirty_(true) {
  value_.x = std::clamp(value_.x, 0.0f, 255.0f);
  value_.y = std::clamp(value_.y, 0.0f, 255.0f);
  value_.z = std::clamp(value_.z, 0.0f, 255.0f);
  value_.w = std::clamp(value_.w, 0.0f, 255.0f);
}

ColorImpl::ColorImpl(const ColorImpl& other)
    : value_(other.value_), dirty_(true) {}

scoped_refptr<ColorImpl> ColorImpl::From(scoped_refptr<Color> host) {
  return static_cast<ColorImpl*>(host.get());
}

void ColorImpl::Set(float red,
                    float green,
                    float blue,
                    float gray,
                    ExceptionState& exception_state) {
  value_.x = std::clamp(red, 0.0f, 255.0f);
  value_.y = std::clamp(green, 0.0f, 255.0f);
  value_.z = std::clamp(blue, 0.0f, 255.0f);
  value_.w = std::clamp(gray, 0.0f, 255.0f);
  dirty_ = true;
}

void ColorImpl::Set(scoped_refptr<Color> other,
                    ExceptionState& exception_state) {
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

float ColorImpl::Get_Red(ExceptionState& exception_state) {
  return value_.x;
}

void ColorImpl::Put_Red(const float& value, ExceptionState& exception_state) {
  if (value_.x != value) {
    dirty_ = true;
    value_.x = std::clamp(value, 0.0f, 255.0f);
  }
}

float ColorImpl::Get_Green(ExceptionState& exception_state) {
  return value_.y;
}

void ColorImpl::Put_Green(const float& value, ExceptionState& exception_state) {
  if (value_.y != value) {
    dirty_ = true;
    value_.y = std::clamp(value, 0.0f, 255.0f);
  }
}

float ColorImpl::Get_Blue(ExceptionState& exception_state) {
  return value_.z;
}

void ColorImpl::Put_Blue(const float& value, ExceptionState& exception_state) {
  if (value_.z != value) {
    dirty_ = true;
    value_.z = std::clamp(value, 0.0f, 255.0f);
  }
}

float ColorImpl::Get_Alpha(ExceptionState& exception_state) {
  return value_.w;
}

void ColorImpl::Put_Alpha(const float& value, ExceptionState& exception_state) {
  if (value_.w != value) {
    dirty_ = true;
    value_.w = std::clamp(value, 0.0f, 255.0f);
  }
}

}  // namespace content
