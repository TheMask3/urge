// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_IO_IOSTREAM_IMPL_H_
#define CONTENT_IO_IOSTREAM_IMPL_H_

#include "SDL3/SDL_iostream.h"

#include "content/context/disposable.h"
#include "content/public/engine_iostream.h"

namespace content {

class IOStreamImpl : public IOStream, public Disposable {
 public:
  IOStreamImpl(SDL_IOStream* stream, std::string buffer);
  ~IOStreamImpl() override;

  IOStreamImpl(const IOStreamImpl&) = delete;
  IOStreamImpl& operator=(const IOStreamImpl&) = delete;

  static scoped_refptr<IOStreamImpl> From(scoped_refptr<IOStream> host);

  SDL_IOStream* GetRawStream() const { return stream_; }

 public:
  void Dispose(ExceptionState& exception_state) override;
  bool IsDisposed(ExceptionState& exception_state) override;
  IOStatus GetStatus(ExceptionState& exception_state) override;
  int64_t GetSize(ExceptionState& exception_state) override;
  int64_t Seek(int64_t offset,
               IOWhence whence,
               ExceptionState& exception_state) override;
  int64_t Tell(ExceptionState& exception_state) override;
  std::string Read(int64_t size, ExceptionState& exception_state) override;
  int64_t Write(const std::string& buffer,
                ExceptionState& exception_state) override;
  bool Flush(ExceptionState& exception_state) override;

 private:
  void OnObjectDisposed() override;
  std::string DisposedObjectName() override { return "IOStream"; }

  SDL_IOStream* stream_;
  std::string buffer_;
};

}  // namespace content

#endif  //! CONTENT_IO_IOSTREAM_IMPL_H_
