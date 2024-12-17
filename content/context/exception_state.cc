// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/context/exception_state.h"

namespace content {

void ExceptionState::ThrowContentError(ExceptionCode exception_code,
                                       const std::string& message) {
  had_exception_ = true;
  code_ = exception_code;
  message_ = message;
}

}  // namespace content
