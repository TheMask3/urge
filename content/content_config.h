// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CONTENT_CONFIG_H_
#define CONTENT_CONTENT_CONFIG_H_

#include "base/buildflags/build.h"
#include "base/buildflags/compiler_specific.h"

#include <optional>

#define URGE_RUNTIME_API
#define URGE_EXPORT_ATTRIBUTE(name, type)       \
  virtual type Get_##name(ExceptionState&) = 0; \
  virtual void Put_##name(const type&, ExceptionState&) = 0
#define URGE_EXPORT_STATIC_ATTRIBUTE(name, type) \
  static type Get_##name(ExceptionState&);       \
  static void Put_##name(const type&, ExceptionState&)

#define URGE_DECLARE_OVERRIDE_ATTRIBUTE(name, type) \
  type Get_##name(ExceptionState&) override;        \
  void Put_##name(const type&, ExceptionState&) override

using ContentFatalInfo = std::optional<std::string_view>;

#endif  //! CONTENT_CONTENT_CONFIG_H_
