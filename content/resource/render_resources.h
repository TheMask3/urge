// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RESOURCE_RENDER_RESOURCES_H_
#define CONTENT_RESOURCE_RENDER_RESOURCES_H_

#include "renderer/device/render_device.h"

namespace content {

struct TextureAgentData {
  // Refcounted webgpu object,
  // storage wrapper with raw pointer for worker usage.
  wgpu::Texture texture;
};

}  // namespace content

#endif  //! CONTENT_RESOURCE_RENDER_RESOURCES_H_
