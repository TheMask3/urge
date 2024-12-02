// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_TONE_H_
#define CONTENT_PUBLIC_ENGINE_TONE_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
// Interface referrence: RPGVXAce.chm
/*--urge()--*/
class URGE_RUNTIME_API Tone : public virtual base::RefCounted<Tone> {
 public:
  virtual ~Tone() = default;

  /*--urge()--*/
  static scoped_refptr<Tone> New();

  /*--urge()--*/
  static scoped_refptr<Tone> New(float red,
                                 float green,
                                 float blue,
                                 float gray);

  /*--urge()--*/
  virtual void Set(float red, float green, float blue, float gray) = 0;

  /*--urge()--*/
  virtual void Set(scoped_refptr<Tone> other) = 0;

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(Red, float);

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(Green, float);

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(Blue, float);

  /*--urge()--*/
  URGE_EXPORT_ATTRIBUTE(Gray, float);
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_TONE_H_
