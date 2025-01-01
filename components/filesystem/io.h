// Copyright 2018-2025 Admenri.
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

  // Open an iostream based on filename,
  // io reader will match extname automatically.
  // When |raw| is true, it will open file with name directly.
  SDL_IOStream* OpenFile(const std::string& filename, bool raw);

  // Check file status in matched path.
  bool IsFileExisted(const std::string& filename);

  // Enumerate content of specific directory.
  bool EnumDirectory(const std::string& path,
                     std::vector<std::string>& out_files);

 private:
  IO();
};

}  // namespace filesystem

#endif  //! COMPONENTS_FILESYSTEM_IO_H_
