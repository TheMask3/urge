// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD - style license that can be
// found in the LICENSE file.

#include "components/filesystem/io_service.h"

#include "SDL3/SDL_system.h"
#include "physfs.h"

#if defined(OS_ANDROID)
#include <jni.h>
#endif

#if HAVE_ARB_ENCRYPTO_SUPPORT
#include "admenri/encryption/encrypt_arb.h"
#endif

namespace filesystem {

namespace {

void ToLower(std::string& str) {
  for (size_t i = 0; i < str.size(); ++i)
    str[i] = tolower(str[i]);
}

const char* FindFileExtName(const char* filename) {
  for (size_t i = strlen(filename); i > 0; --i) {
    if (filename[i] == '/')
      break;
    if (filename[i] == '.')
      return filename + i + 1;
  }

  return nullptr;
}

inline PHYSFS_File* PHYSPtr(void* udata) {
  return static_cast<PHYSFS_File*>(udata);
}

Sint64 PHYS_RWopsSize(void* userdata) {
  PHYSFS_File* f = PHYSPtr(userdata);
  if (!f)
    return -1;

  return PHYSFS_fileLength(f);
}

Sint64 PHYS_RWopsSeek(void* userdata, int64_t offset, SDL_IOWhence whence) {
  PHYSFS_File* f = PHYSPtr(userdata);
  if (!f)
    return -1;

  int64_t base;

  switch (whence) {
    default:
    case SDL_IO_SEEK_SET:
      base = 0;
      break;
    case SDL_IO_SEEK_CUR:
      base = PHYSFS_tell(f);
      break;
    case SDL_IO_SEEK_END:
      base = PHYSFS_fileLength(f);
      break;
  }

  int result = PHYSFS_seek(f, base + offset);
  return (result != 0) ? PHYSFS_tell(f) : -1;
}

size_t PHYS_RWopsRead(void* userdata,
                      void* buffer,
                      size_t size,
                      SDL_IOStatus* status) {
  PHYSFS_File* f = PHYSPtr(userdata);
  if (!f)
    return 0;

  PHYSFS_sint64 result = PHYSFS_readBytes(f, buffer, size);
  return (result != -1) ? result : 0;
}

size_t PHYS_RWopsWrite(void* userdata,
                       const void* buffer,
                       size_t size,
                       SDL_IOStatus* status) {
  PHYSFS_File* f = PHYSPtr(userdata);
  if (!f)
    return 0;

  PHYSFS_sint64 result = PHYSFS_writeBytes(f, buffer, size);

  return (result != -1) ? result : 0;
}

bool PHYS_RWopsClose(void* userdata) {
  PHYSFS_File* f = PHYSPtr(userdata);
  if (!f)
    return false;

  int result = PHYSFS_close(f);
  return result != 0;
}

SDL_IOStream* WrapperRWops(PHYSFS_File* handle) {
  SDL_IOStreamInterface iface;
  SDL_INIT_INTERFACE(&iface);

  iface.size = PHYS_RWopsSize;
  iface.seek = PHYS_RWopsSeek;
  iface.read = PHYS_RWopsRead;
  iface.write = PHYS_RWopsWrite;
  iface.close = PHYS_RWopsClose;

  return SDL_OpenIO(&iface, handle);
}

struct OpenReadEnumData {
  IOService::OpenCallback callback;
  std::string full;
  std::string dir;
  std::string file;
  std::string ext;

  bool search_complete = false;
  int match_count = 0;

  std::string physfs_error;

  OpenReadEnumData() = default;
};

PHYSFS_EnumerateCallbackResult OpenReadEnumCallback(void* data,
                                                    const char* origdir,
                                                    const char* fname) {
  OpenReadEnumData* enum_data = static_cast<OpenReadEnumData*>(data);
  std::string filename(fname);
  ToLower(filename);

  if (enum_data->search_complete)
    return PHYSFS_ENUM_STOP;

  size_t it = filename.rfind(enum_data->file);
  if (it == std::string::npos)
    return PHYSFS_ENUM_OK;

  const char last = filename[enum_data->file.size()];
  if (last != '.' && last != '/')
    return PHYSFS_ENUM_OK;

  std::string fullpath;
  if (*origdir) {
    fullpath += std::string(origdir);
    fullpath += "/";
  }
  fullpath += fname;

  PHYSFS_File* file = PHYSFS_openRead(fullpath.c_str());
  if (!file) {
    enum_data->search_complete = true;
    enum_data->physfs_error = PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

    return PHYSFS_ENUM_ERROR;
  }

  SDL_IOStream* ops = WrapperRWops(file);
  // Free on user callback side
  if (enum_data->callback.Run(ops, FindFileExtName(filename.c_str())))
    enum_data->search_complete = true;

  enum_data->match_count++;
  return PHYSFS_ENUM_OK;
}

}  // namespace

IOService::IOService(const std::string& argv0) {
  const char* init_data = argv0.c_str();

#if defined(OS_ANDROID)
  PHYSFS_AndroidInit ainit;
  ainit.jnienv = SDL_GetAndroidJNIEnv();
  ainit.context = SDL_GetAndroidActivity();
  init_data = (const char*)&ainit;
#endif

  if (!PHYSFS_init(init_data)) {
    LOG(INFO) << "[IOService] Failed to load Physfs.";
  } else {
    LOG(INFO) << "[IOService] BasePath: " << PHYSFS_getBaseDir();
  }

#if HAVE_ARB_ENCRYPTO_SUPPORT
  if (PHYSFS_registerArchiver(&admenri::admenri_archiver_arb))
    LOG(INFO) << "[ARB] Enable ARB crypto library.";
#endif

#if defined(OS_ANDROID)
  PHYSFS_mount(PHYSFS_getBaseDir(), nullptr, 1);
  PHYSFS_setRoot(PHYSFS_getBaseDir(), "/assets");
#endif
}

IOService::~IOService() {
  if (!PHYSFS_deinit())
    LOG(INFO) << "[IOService] Failed to unload Physfs.";
}

void IOService::AddLoadPath(const std::string& path) {
  if (!PHYSFS_mount(path.c_str(), nullptr, 1))
    LOG(INFO) << "[IOService] Failed to add path: " << path;
}

bool IOService::Exists(const std::string& filename) {
  return PHYSFS_exists(filename.c_str());
}

std::vector<std::string> IOService::EnumDir(const std::string& dir) {
  std::vector<std::string> files;

  PHYSFS_enumerate(
      dir.c_str(),
      [](void* data, const char* origdir,
         const char* fname) -> PHYSFS_EnumerateCallbackResult {
        std::vector<std::string>* files =
            static_cast<std::vector<std::string>*>(data);
        files->push_back(fname);
        return PHYSFS_ENUM_OK;
      },
      &files);

  return files;
}

void IOService::OpenRead(const std::string& file_path,
                         OpenCallback callback,
                         IOState* io_state) {
  std::string filename(file_path);
  std::string dir, file, ext;

  size_t last_slash_pos = filename.find_last_of('/');
  if (last_slash_pos != std::string::npos) {
    dir = filename.substr(0, last_slash_pos);
    file = filename.substr(last_slash_pos + 1);
  } else
    file = filename;

  size_t last_dot_pos = file.find_last_of('.');
  if (last_dot_pos != std::string::npos) {
    ext = file.substr(last_dot_pos + 1);
    file = file.substr(0, last_dot_pos);
  }

  OpenReadEnumData data;
  data.callback = callback;
  data.full = filename;
  data.dir = dir;
  data.file = file;
  ToLower(data.file);
  data.ext = ext;

  if (!PHYSFS_enumerate(dir.c_str(), OpenReadEnumCallback, &data))
    LOG(INFO) << "[IOService] "
              << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());

  if (!data.physfs_error.empty() && io_state) {
    io_state->error_count++;
    io_state->error_message = data.physfs_error;
    return;
  }

  if (data.match_count <= 0 && io_state) {
    io_state->error_count++;
    io_state->error_message = "No file match: " + file_path;
    return;
  }
}

SDL_IOStream* IOService::OpenReadRaw(const std::string& filename,
                                     IOState* io_state) {
  PHYSFS_File* file = PHYSFS_openRead(filename.c_str());
  if (!file) {
    if (io_state) {
      io_state->error_count++;
      io_state->error_message =
          std::string(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())) + ": " +
          filename;
    }

    return nullptr;
  }

  return WrapperRWops(file);
}

}  // namespace filesystem
