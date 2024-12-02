// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_TONE_IMPL_H_
#define CONTENT_COMMON_TONE_IMPL_H_

#include <tuple>

#include "base/math/vector.h"
#include "content/public/engine_tone.h"

namespace content {

class ToneImpl : public Tone {
 public:
  ToneImpl(const base::Vec4& value);
  ~ToneImpl() override = default;

  ToneImpl(const ToneImpl&) = delete;
  ToneImpl& operator=(const ToneImpl&) = delete;

  static scoped_refptr<ToneImpl> From(scoped_refptr<Tone> host);

  void Set(float red, float green, float blue, float gray) override;
  void Set(scoped_refptr<Tone> other) override;

  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Red, float);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Green, float);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Blue, float);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Gray, float);

  std::pair<bool, base::Vec4> FetchUpdateRequiredAndData();

 private:
  base::Vec4 value_;
  base::Vec4 norm_;
  bool dirty_;
};

}  // namespace content

#endif  //! CONTENT_COMMON_TONE_IMPL_H_
