// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_FILESYSTEM_IO_H_
#define COMPONENTS_FILESYSTEM_IO_H_

#include "SDL3/SDL_iostream.h"

#include <memory>
#include <string>
#include <vector>

namespace filesystem {

class IO {
 public:
  ~IO();

  IO(const IO&) = delete;
  IO& operator=(const IO&) = delete;

  // Make filesystem io instance
  static std::unique_ptr<IO> Create();

  // i/o file abstract interface
  SDL_IOStream* OpenFile(const std::string& filename, bool raw);
  bool IsFileExisted(const std::string& filename);
  void EnumFiles(const std::string& path, std::vector<std::string>& out_files);

 private:
  IO();
};

}  // namespace filesystem

#endif  //! COMPONENTS_FILESYSTEM_IO_H_
