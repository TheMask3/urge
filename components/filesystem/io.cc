// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/filesystem/io.h"

namespace filesystem {

IO::~IO() {}

std::unique_ptr<IO> IO::Create() {
  return std::unique_ptr<IO>(new IO());
}

SDL_IOStream* IO::OpenFile(const std::string& filename, bool raw) {
  return nullptr;
}

bool IO::IsFileExisted(const std::string& filename) {
  return false;
}

void IO::EnumFiles(const std::string& path,
                   std::vector<std::string>& out_files) {}

}  // namespace filesystem
