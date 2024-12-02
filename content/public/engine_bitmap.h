// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_BITMAP_H_
#define CONTENT_PUBLIC_ENGINE_BITMAP_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"
#include "content/public/engine_color.h"
#include "content/public/engine_font.h"
#include "content/public/engine_rect.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
// Interface referrence: RPGVXAce.chm
/*--urge()--*/
class URGE_RUNTIME_API Bitmap : public virtual base::RefCounted<Bitmap> {
 public:
  virtual ~Bitmap() = default;

  /*--urge()--*/
  static scoped_refptr<Bitmap> New(const std::string& filename);

  /*--urge()--*/
  static scoped_refptr<Bitmap> New(uint32_t width, uint32_t height);

  /*--urge()--*/
  virtual void Dispose() = 0;

  /*--urge()--*/
  virtual bool IsDisposed() = 0;

  /*--urge()--*/
  virtual uint32_t Width() = 0;

  /*--urge()--*/
  virtual uint32_t Height() = 0;

  /*--urge(alias_name:rect)--*/
  virtual scoped_refptr<Rect> GetRect() = 0;

  /*--urge()--*/
  virtual void Blt(int32_t x,
                   int32_t y,
                   scoped_refptr<Bitmap> src_bitmap,
                   scoped_refptr<Rect> src_rect,
                   uint32_t opacity) = 0;

  /*--urge()--*/
  virtual void StretchBlt(scoped_refptr<Rect> dest_rect,
                          scoped_refptr<Bitmap> src_bitmap,
                          scoped_refptr<Rect> src_rect,
                          uint32_t opacity) = 0;

  /*--urge()--*/
  virtual void FillRect(int32_t x,
                        int32_t y,
                        uint32_t width,
                        uint32_t height,
                        scoped_refptr<Color> color) = 0;

  /*--urge()--*/
  virtual void FillRect(scoped_refptr<Rect> rect,
                        scoped_refptr<Color> color) = 0;

  /*--urge()--*/
  virtual void GradientFillRect(int32_t x,
                                int32_t y,
                                uint32_t width,
                                uint32_t height,
                                scoped_refptr<Color> color1,
                                scoped_refptr<Color> color2,
                                bool vertical) = 0;

  /*--urge()--*/
  virtual void GradientFillRect(scoped_refptr<Rect> rect,
                                scoped_refptr<Color> color1,
                                scoped_refptr<Color> color2,
                                bool vertical) = 0;

  /*--urge()--*/
  virtual void Clear() = 0;

  /*--urge()--*/
  virtual void ClearRect(int32_t x,
                         int32_t y,
                         uint32_t width,
                         uint32_t height) = 0;

  /*--urge()--*/
  virtual void ClearRect(scoped_refptr<Rect> rect) = 0;

  /*--urge()--*/
  virtual scoped_refptr<Color> GetPixel(int32_t x, int32_t y) = 0;

  /*--urge()--*/
  virtual void SetPixel(int32_t x, int32_t y, scoped_refptr<Color> color) = 0;

  /*--urge()--*/
  virtual void HueChange(int32_t hue) = 0;

  /*--urge()--*/
  virtual void Blur() = 0;

  /*--urge()--*/
  virtual void RadialBlur(int32_t angle, int32_t division) = 0;

  /*--urge()--*/
  virtual void DrawText(int32_t x,
                        int32_t y,
                        uint32_t width,
                        uint32_t height,
                        const std::string& str,
                        int32_t align) = 0;

  /*--urge()--*/
  virtual void DrawText(scoped_refptr<Rect> rect,
                        const std::string& str,
                        int32_t align) = 0;

  /*--urge()--*/
  virtual scoped_refptr<Rect> TextSize(const std::string& str) = 0;

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(Font, scoped_refptr<Font>);
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_BITMAP_H_
