// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_IOSTREAM_H_
#define CONTENT_PUBLIC_ENGINE_IOSTREAM_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"
#include "content/context/exception_state.h"
#include "content/context/execution_context.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
/*--urge(name:IOStream)--*/
class URGE_RUNTIME_API IOStream : public base::RefCounted<IOStream> {
 public:
  virtual ~IOStream() = default;

  /*--urge(name:IOStatus)--*/
  enum IOStatus {
    STATUS_READY,
    STATUS_ERROR,
    STATUS_EOF,
    STATUS_NOT_READY,
    STATUS_READONLY,
    STATUS_WRITEONLY,
  };

  /*--urge(name:IOWhence)--*/
  enum IOWhence {
    IOSEEK_SET,
    IOSEEK_CUR,
    IOSEEK_END,
  };

  /*--urge(name:from_filesystem)--*/
  static scoped_refptr<IOStream> FromFileSystem(
      ExecutionContext* execution_context,
      const std::string& filename,
      const std::string& mode,
      ExceptionState& exception_state);

  /*--urge(name:from_iosystem)--*/
  static scoped_refptr<IOStream> FromIOSystem(
      ExecutionContext* execution_context,
      const std::string& filename,
      ExceptionState& exception_state);

  /*--urge(name:from_memory)--*/
  static scoped_refptr<IOStream> FromMemory(ExecutionContext* execution_context,
                                            const std::string& buffer,
                                            ExceptionState& exception_state);

  /*--urge(name:dispose)--*/
  virtual void Dispose(ExceptionState& exception_state) = 0;

  /*--urge(name:disposed?)--*/
  virtual bool IsDisposed(ExceptionState& exception_state) = 0;

  /*--urge(name:status)--*/
  virtual IOStatus GetStatus(ExceptionState& exception_state) = 0;

  /*--urge(name:size)--*/
  virtual int64_t GetSize(ExceptionState& exception_state) = 0;

  /*--urge(name:seek)--*/
  virtual int64_t Seek(int64_t offset,
                       IOWhence whence,
                       ExceptionState& exception_state) = 0;

  /*--urge(name:tell)--*/
  virtual int64_t Tell(ExceptionState& exception_state) = 0;

  /*--urge(name:read)--*/
  virtual std::string Read(int64_t size, ExceptionState& exception_state) = 0;

  /*--urge(name:write)--*/
  virtual int64_t Write(const std::string& buffer,
                        ExceptionState& exception_state) = 0;

  /*--urge(name:flush)--*/
  virtual bool Flush(ExceptionState& exception_state) = 0;
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_IOSTREAM_H_
