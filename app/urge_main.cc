// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SDL3/SDL_main.h"
#include "SDL3/SDL_messagebox.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "binding/mri/mri_main.h"
#include "components/filesystem/io_service.h"
#include "content/components/font_context.h"
#include "content/profile/i18n_profile.h"
#include "content/worker/content_runner.h"
#include "ui/widget/widget.h"

#if defined(OS_ANDROID)
#include <jni.h>
#include <sys/system_properties.h>
#include <unistd.h>
#include <filesystem>
#endif

#if defined(OS_WIN)
#include <windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif  //! OS_WIN

int main(int argc, char* argv[]) {
#if defined(OS_WIN)
  ::SetConsoleOutputCP(CP_UTF8);
#endif  //! defined(OS_WIN)

#if defined(OS_ANDROID)
  // Get GAME_PATH string field from JNI (MainActivity.java)
  JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
  jobject activity = (jobject)SDL_GetAndroidActivity();
  jclass activity_klass = env->GetObjectClass(activity);
  jfieldID field_game_path =
      env->GetStaticFieldID(activity_klass, "GAME_PATH", "Ljava/lang/String;");
  jstring java_string_game_path =
      (jstring)env->GetStaticObjectField(activity_klass, field_game_path);
  const char* game_data_dir = env->GetStringUTFChars(java_string_game_path, 0);

  // Set and ensure current directory
  std::filesystem::path std_path(game_data_dir);
  if (!std::filesystem::exists(std_path) ||
      !std::filesystem::is_directory(std_path))
    std::filesystem::create_directories(std_path);

  std::filesystem::current_path(std_path);
  if (std::filesystem::equivalent(std::filesystem::current_path(), std_path))
    LOG(INFO) << "[Android] Base directory: " << game_data_dir;

  env->ReleaseStringUTFChars(java_string_game_path, game_data_dir);
  env->DeleteLocalRef(java_string_game_path);
  env->DeleteLocalRef(activity_klass);

  // Fixed configure file
  std::string app = "Game";
  std::string ini = app + ".ini";
#else
  std::string app(argv[0]);
  for (size_t i = 0; i < app.size(); ++i)
    if (app[i] == '\\')
      app[i] = '/';

  auto last_sep = app.find_last_of('/');
  if (last_sep != std::string::npos)
    app = app.substr(last_sep + 1);

  LOG(INFO) << "[App] Path: " << app;

  last_sep = app.find_last_of('.');
  if (last_sep != std::string::npos)
    app = app.substr(0, last_sep);
  std::string ini = app + ".ini";
#endif  //! defined(OS_ANDROID)

  LOG(INFO) << "[App] Configure: " << ini;

  // Initialize filesystem
  std::unique_ptr<filesystem::IOService> io_service =
      std::make_unique<filesystem::IOService>(argv[0]);
  io_service->AddLoadPath(".");

  filesystem::IOState io_state;
  SDL_IOStream* inifile = io_service->OpenReadRaw(ini, &io_state);
  if (io_state.error_count) {
    LOG(INFO) << "[App] Warning: " << io_state.error_message;
  }

  // Initialize profile
  std::unique_ptr<content::ContentProfile> profile =
      content::ContentProfile::MakeFrom(app, inifile);
  profile->LoadCommandLine(argc, argv);

  if (!profile->LoadConfigure(app)) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "URGE",
                             "Error when parse configure file.", nullptr);
    return 1;
  }

  // Setup encryption resource package
  std::string app_package = app + ".arb";
  io_service->AddLoadPath(app_package);

  // Disable IME on Windows
#if defined(OS_WIN)
  if (profile->disable_ime) {
    LOG(INFO) << "[Windows] Disable process IME.";
    ::ImmDisableIME(TRUE);
  }
#endif

  // Setup SDL init params
  SDL_SetHint(SDL_HINT_ORIENTATIONS, profile->orientation.c_str());
  SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
  SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  TTF_Init();

  {
    // Initialize i18n profile
    auto* i18n_xml_stream =
        io_service->OpenReadRaw(profile->i18n_xml_path, nullptr);
    auto i18n_profile = content::I18NProfile::MakeForStream(i18n_xml_stream);

    // Initialize font context
    auto font_context = std::make_unique<content::ScopedFontData>(
        io_service.get(), profile->default_font_path);

    {
      // Initialize engine main widget
      std::unique_ptr<ui::Widget> widget(new ui::Widget(true));
      ui::Widget::InitParams widget_params;
#if defined(OS_LINUX)
      widget_params.opengl = profile->driver_backend == "OPENGL";
#endif
      widget_params.size = profile->window_size;
      widget_params.resizable = true;
      widget_params.hpixeldensity = true;
      widget_params.fullscreen =
#if defined(OS_ANDROID)
          true;
#else
          profile->fullscreen;
#endif
      widget_params.title = profile->window_title;
      widget->Init(std::move(widget_params));

      // Create worker thread
      auto render_worker = base::ThreadWorker::Create();

      // Setup content runner module
      content::ContentRunner::InitParams content_params;
      content_params.profile = profile.get();
      content_params.io_service = io_service.get();
      content_params.font_context = font_context.get();
      content_params.i18n_profile = i18n_profile.get();
      content_params.window = widget->AsWeakPtr();
      content_params.render_worker = render_worker.get();
      content_params.entry = std::make_unique<binding::BindingEngineMri>();

      std::unique_ptr<content::ContentRunner> runner =
          content::ContentRunner::Create(std::move(content_params));
      runner->RunMainLoop();

      // Finalize modules at end
      runner.reset();
      render_worker.reset();
      widget.reset();
    }

    // Release resources
    font_context.reset();
    i18n_profile.reset();
  }

  // Release objects
  profile.reset();
  io_service.reset();

  TTF_Quit();
  SDL_Quit();

  return 0;
}
