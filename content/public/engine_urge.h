// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_URGE_H_
#define CONTENT_PUBLIC_ENGINE_URGE_H_

#include <stdint.h>

#include "base/memory/ref_counted.h"
#include "content/content_config.h"
#include "content/context/exception_state.h"
#include "content/context/execution_context.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
/*--urge(name:URGE,is_module)--*/
class URGE_RUNTIME_API URGE : public base::RefCounted<URGE> {
 public:
  virtual ~URGE() = default;

  /*--urge(name:platform)--*/
  virtual std::string GetPlatform(ExceptionState& exception_state) = 0;

  /*--urge(name:open_url)--*/
  virtual void OpenURL(const std::string& path,
                       ExceptionState& exception_state) = 0;

  /*--urge(name:gets)--*/
  virtual std::string Gets(ExceptionState& exception_state) = 0;

  /*--urge(name:puts)--*/
  virtual void Puts(const std::string& message,
                    ExceptionState& exception_state) = 0;

  /*--urge(name:alert)--*/
  virtual void Alert(const std::string& message,
                     ExceptionState& exception_state) = 0;

  /*--urge(name:confirm)--*/
  virtual bool Confirm(const std::string& message,
                       ExceptionState& exception_state) = 0;

  /*--urge(name:add_load_path)--*/
  virtual bool AddLoadPath(const std::string& new_path,
                           const std::string& mount_point,
                           bool append_to_path,
                           ExceptionState& exception_state) = 0;

  /*--urge(name:remove_load_path)--*/
  virtual bool RemoveLoadPath(const std::string& old_path,
                              ExceptionState& exception_state) = 0;

  /*--urge(name:file_exist?)--*/
  virtual bool IsFileExisted(const std::string& filepath,
                             ExceptionState& exception_state) = 0;

  /*--urge(name:enum_directory)--*/
  virtual std::vector<std::string> EnumDirectory(
      const std::string& dirpath,
      ExceptionState& exception_state) = 0;
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_URGE_H_
