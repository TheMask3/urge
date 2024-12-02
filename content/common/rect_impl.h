// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_RECT_IMPL_H_
#define CONTENT_COMMON_RECT_IMPL_H_

#include "base/math/rectangle.h"
#include "content/public/engine_rect.h"

namespace content {

class RectImpl : public Rect {
 public:
  RectImpl(const base::Rect& rect);
  ~RectImpl() override = default;

  RectImpl(const RectImpl&) = delete;
  RectImpl& operator=(const RectImpl&) = delete;

  static scoped_refptr<RectImpl> From(scoped_refptr<Rect> host);

  void Set(int32_t x, int32_t y, int32_t width, int32_t height) override;
  void Set(scoped_refptr<Rect> other) override;
  void Empty() override;

  URGE_DECLARE_OVERRIDE_ATTRIBUTE(X, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Y, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Width, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Height, int32_t);

  bool FetchDirtyStatus();
  base::Rect AsBaseRect();

 private:
  base::Rect rect_;
  bool dirty_;
};

}  // namespace content

#endif  //! CONTENT_COMMON_RECT_IMPL_H_
