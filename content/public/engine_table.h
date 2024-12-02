// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_TABLE_H_
#define CONTENT_PUBLIC_ENGINE_TABLE_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
// Interface referrence: RPGVXAce.chm
/*--urge()--*/
class URGE_RUNTIME_API Table : public virtual base::RefCounted<Table> {
 public:
  virtual ~Table() = default;

  /*--urge()--*/
  static scoped_refptr<Table> New(uint32_t xsize,
                                  uint32_t ysize,
                                  uint32_t zsize);

  /*--urge()--*/
  virtual void Resize(uint32_t xsize, uint32_t ysize, uint32_t zsize) = 0;

  /*--urge()--*/
  virtual uint32_t Xsize() = 0;

  /*--urge()--*/
  virtual uint32_t Ysize() = 0;

  /*--urge()--*/
  virtual uint32_t Zsize() = 0;

  /*--urge(alias_name:[])--*/
  virtual int16_t Get(uint32_t x) = 0;

  /*--urge(alias_name:[])--*/
  virtual int16_t Get(uint32_t x, uint32_t y) = 0;

  /*--urge(alias_name:[])--*/
  virtual int16_t Get(uint32_t x, uint32_t y, uint32_t z) = 0;

  /*--urge(alias_name:[]=)--*/
  virtual void Put(uint32_t x, int16_t value) = 0;

  /*--urge(alias_name:[]=)--*/
  virtual void Put(uint32_t x, uint32_t y, int16_t value) = 0;

  /*--urge(alias_name:[]=)--*/
  virtual void Put(uint32_t x, uint32_t y, uint32_t z, int16_t value) = 0;
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_RECT_H_
