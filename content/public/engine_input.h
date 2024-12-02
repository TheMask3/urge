// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_INPUT_H_
#define CONTENT_PUBLIC_ENGINE_INPUT_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
// Interface referrence: RPGVXAce.chm
/*--urge()--*/
class URGE_RUNTIME_API Input : public virtual base::RefCounted<Input> {
 public:
  virtual ~Input() = default;

  /*--urge()--*/
  virtual void Update() = 0;

  /*--urge()--*/
  virtual bool IsPressed(const std::string& sym) = 0;

  /*--urge()--*/
  virtual bool IsTriggered(const std::string& sym) = 0;

  /*--urge()--*/
  virtual bool IsRepeated(const std::string& sym) = 0;

  /*--urge()--*/
  virtual int32_t Dir4() = 0;

  /*--urge()--*/
  virtual int32_t Dir8() = 0;
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_INPUT_H_
