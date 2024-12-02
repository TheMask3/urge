// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_GRAPHICS_H_
#define CONTENT_PUBLIC_ENGINE_GRAPHICS_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"
#include "content/public/engine_bitmap.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
// Interface referrence: RPGVXAce.chm
/*--urge()--*/
class URGE_RUNTIME_API Graphics : public virtual base::RefCounted<Graphics> {
 public:
  virtual ~Graphics() = default;

  /*--urge()--*/
  virtual void Update() = 0;

  /*--urge()--*/
  virtual void Wait(uint32_t duration) = 0;

  /*--urge()--*/
  virtual void FadeOut(uint32_t duration) = 0;

  /*--urge()--*/
  virtual void FadeIn(uint32_t duration) = 0;

  /*--urge()--*/
  virtual void Freeze() = 0;

  /*--urge()--*/
  virtual void Transition(uint32_t duration,
                          const std::string& filename,
                          uint32_t vague) = 0;

  /*--urge()--*/
  virtual scoped_refptr<Bitmap> SnapToBitmap() = 0;

  /*--urge()--*/
  virtual void FrameReset() = 0;

  /*--urge()--*/
  virtual uint32_t Width() = 0;

  /*--urge()--*/
  virtual uint32_t Height() = 0;

  /*--urge()--*/
  virtual void ResizeScreen(uint32_t width, uint32_t height) = 0;

  /*--urge()--*/
  virtual void PlayMovie(const std::string& filename) = 0;

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(FrameRate, uint32_t);

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(FrameCount, uint32_t);

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(Brightness, uint32_t);
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_GRAPHICS_H_
