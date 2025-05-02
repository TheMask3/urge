// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_ENGINE_AUDIO_H_
#define CONTENT_PUBLIC_ENGINE_AUDIO_H_

#include "base/memory/ref_counted.h"
#include "content/content_config.h"
#include "content/context/exception_state.h"
#include "content/context/execution_context.h"

namespace content {

// IDL generator format:
// Inhert: refcounted only.
// Interface reference: RGSS Reference
/*--urge(name:Audio,is_module)--*/
class URGE_RUNTIME_API Audio : public base::RefCounted<Audio> {
 public:
  virtual ~Audio() = default;

  /*--urge(name:setup_midi)--*/
  virtual void SetupMIDI(ExceptionState& exception_state) = 0;

  /*--urge(name:bgm_play,optional:volume=80,optional:pitch=100,optional:pos=0)--*/
  virtual void BGMPlay(const std::string& filename,
                       int32_t volume,
                       int32_t pitch,
                       int32_t pos,
                       ExceptionState& exception_state) = 0;

  /*--urge(name:bgm_stop)--*/
  virtual void BGMStop(ExceptionState& exception_state) = 0;

  /*--urge(name:bgm_fade)--*/
  virtual void BGMFade(int32_t time, ExceptionState& exception_state) = 0;

  /*--urge(name:bgm_pos)--*/
  virtual int32_t BGMPos(ExceptionState& exception_state) = 0;

  /*--urge(name:bgs_play,optional:volume=80,optional:pitch=100,optional:pos=0)--*/
  virtual void BGSPlay(const std::string& filename,
                       int32_t volume,
                       int32_t pitch,
                       int32_t pos,
                       ExceptionState& exception_state) = 0;

  /*--urge(name:bgs_stop)--*/
  virtual void BGSStop(ExceptionState& exception_state) = 0;

  /*--urge(name:bgs_fade)--*/
  virtual void BGSFade(int32_t time, ExceptionState& exception_state) = 0;

  /*--urge(name:bgs_pos)--*/
  virtual int32_t BGSPos(ExceptionState& exception_state) = 0;

  /*--urge(name:me_play,optional:volume=80,optional:pitch=100)--*/
  virtual void MEPlay(const std::string& filename,
                      int32_t volume,
                      int32_t pitch,
                      ExceptionState& exception_state) = 0;

  /*--urge(name:me_stop)--*/
  virtual void MEStop(ExceptionState& exception_state) = 0;

  /*--urge(name:me_fade)--*/
  virtual void MEFade(int32_t time, ExceptionState& exception_state) = 0;

  /*--urge(name:se_play,optional:volume=80,optional:pitch=100,optional:pan=0)--*/
  virtual void SEPlay(const std::string& filename,
                      int32_t volume,
                      int32_t pitch,
                      int32_t pan,
                      ExceptionState& exception_state) = 0;

  /*--urge(name:se_stop)--*/
  virtual void SEStop(ExceptionState& exception_state) = 0;

  /*--urge(name:reset)--*/
  virtual void Reset(ExceptionState& exception_state) = 0;
};

}  // namespace content

#endif  //! CONTENT_PUBLIC_ENGINE_INPUT_H_
