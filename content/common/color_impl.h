// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_COLOR_IMPL_H_
#define CONTENT_COMMON_COLOR_IMPL_H_

#include <tuple>

#include "SDL3/SDL_pixels.h"

#include "base/math/vector.h"
#include "content/public/engine_color.h"

namespace content {

class ColorImpl : public Color {
 public:
  ColorImpl(const base::Vec4& value);
  ColorImpl(const ColorImpl& other);
  ~ColorImpl() override = default;

  static scoped_refptr<ColorImpl> From(scoped_refptr<Color> host);

  void Set(float red,
           float green,
           float blue,
           float gray,
           ExceptionState& exception_state) override;
  void Set(scoped_refptr<Color> other,
           ExceptionState& exception_state) override;

  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Red, float);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Green, float);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Blue, float);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Alpha, float);

  SDL_Color AsSDLColor();
  std::pair<bool, base::Vec4> FetchUpdateRequiredAndData();

 private:
  base::Vec4 value_;
  base::Vec4 norm_;
  bool dirty_;
};

}  // namespace content

#endif  //! CONTENT_COMMON_COLOR_IMPL_H_
