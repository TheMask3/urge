// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/render/tilemap2_impl.h"

#include "renderer/utils/texture_utils.h"

namespace content {

namespace {

const base::RectF kAutotileSrcRegular[] = {
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 0.5f},
};

const base::RectF kAutotileSrcTable[] = {
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {1.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {0.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {0.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 2.0f, 0.5f, 0.5f}, {1.5f, 2.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 2.5f, 0.5f, 0.5f}, {1.5f, 2.5f, 0.5f, 0.5f},
    {0.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 0.5f},
};

const base::RectF kAutotileSrcWall[] = {
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {0.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 0.5f, 0.5f, 0.5f}, {1.5f, 0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {0.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.0f, 0.5f, 0.5f}, {0.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {0.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 1.0f, 0.5f, 0.5f}, {1.5f, 1.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {1.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {1.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
    {0.0f, 0.0f, 0.5f, 0.5f}, {1.5f, 0.0f, 0.5f, 0.5f},
    {0.0f, 1.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 0.5f, 0.5f},
};

const base::RectF kAutotileSrcWaterfall[] = {
    {1.0f, 0.0f, 0.5f, 1.0f}, {0.5f, 0.0f, 0.5f, 1.0f},
    {0.0f, 0.0f, 0.5f, 1.0f}, {0.5f, 0.0f, 0.5f, 1.0f},
    {1.0f, 0.0f, 0.5f, 1.0f}, {1.5f, 0.0f, 0.5f, 1.0f},
    {0.0f, 0.0f, 0.5f, 1.0f}, {1.5f, 0.0f, 0.5f, 1.0f},
};

const Tilemap2Impl::AtlasBlock kTilemapAtlas[] = {
    /* A1 tilemap */
    {Tilemap2Impl::BitmapID::TILE_A1, {0, 0, 6, 6}, {0, 0}},
    {Tilemap2Impl::BitmapID::TILE_A1, {8, 0, 6, 6}, {6, 0}},
    {Tilemap2Impl::BitmapID::TILE_A1, {0, 6, 6, 6}, {0, 6}},
    {Tilemap2Impl::BitmapID::TILE_A1, {8, 6, 6, 6}, {6, 6}},
    {Tilemap2Impl::BitmapID::TILE_A1, {6, 0, 2, 12}, {12, 0}},
    {Tilemap2Impl::BitmapID::TILE_A1, {14, 0, 2, 12}, {14, 0}},

    /* A2 tilemap */
    {Tilemap2Impl::BitmapID::TILE_A2, {0, 0, 16, 12}, {16, 0}},

    /* A3 tilemap */
    {Tilemap2Impl::BitmapID::TILE_A3, {0, 0, 16, 8}, {0, 12}},

    /* A4 tilemap */
    {Tilemap2Impl::BitmapID::TILE_A4, {0, 0, 16, 15}, {16, 12}},

    /* A5 tilemap */
    {Tilemap2Impl::BitmapID::TILE_A5, {0, 0, 8, 8}, {0, 20}},
    {Tilemap2Impl::BitmapID::TILE_A5, {0, 8, 8, 8}, {8, 20}},

    /* B tilemap */
    {Tilemap2Impl::BitmapID::TILE_B, {0, 0, 16, 16}, {32, 0}},

    /* C tilemap */
    {Tilemap2Impl::BitmapID::TILE_C, {0, 0, 16, 16}, {48, 0}},

    /* D tilemap */
    {Tilemap2Impl::BitmapID::TILE_D, {0, 0, 16, 16}, {32, 16}},

    /* E tilemap */
    {Tilemap2Impl::BitmapID::TILE_E, {0, 0, 16, 16}, {48, 16}},
};

const base::Rect kShadowAtlasArea = {16, 27, 16, 1};

SDL_Surface* CreateShadowSet(int32_t tilesize) {
  std::vector<SDL_Rect> rects;
  SDL_Surface* surf = SDL_CreateSurface(kShadowAtlasArea.width * tilesize,
                                        kShadowAtlasArea.height * tilesize,
                                        SDL_PIXELFORMAT_ABGR8888);

  for (int32_t i = 0; i < 16; ++i) {
    int32_t offset = i * tilesize;
    if (i & 0x1)  // Left Top
      rects.push_back({offset, 0, tilesize / 2, tilesize / 2});
    if (i & 0x2)  // Right Top
      rects.push_back({offset + tilesize / 2, 0, tilesize / 2, tilesize / 2});
    if (i & 0x4)  // Left Bottom
      rects.push_back({offset, tilesize / 2, tilesize / 2, tilesize / 2});
    if (i & 0x8)  // Right Bottom
      rects.push_back(
          {offset + tilesize / 2, tilesize / 2, tilesize / 2, tilesize / 2});
  }

  const auto* pixel_detail = SDL_GetPixelFormatDetails(surf->format);
  const uint32_t color = SDL_MapRGBA(pixel_detail, nullptr, 0, 0, 0, 128);
  SDL_FillSurfaceRects(surf, rects.data(), rects.size(), color);

  return surf;
}

void GPUCreateTilemapInternal(renderer::RenderDevice* device,
                              Tilemap2Agent* agent) {
  agent->batch = renderer::QuadBatch::Make(**device);
  agent->shader_binding = device->GetPipelines()->tilemap2.CreateBinding();

  Diligent::CreateUniformBuffer(
      **device, sizeof(renderer::Binding_Tilemap::Params), "tilemap.uniform",
      &agent->uniform_buffer, Diligent::USAGE_DEFAULT,
      Diligent::BIND_UNIFORM_BUFFER, Diligent::CPU_ACCESS_NONE);
}

void GPUDestroyTilemapInternal(Tilemap2Agent* agent) {
  delete agent;
}

void GPUMakeAtlasInternal(
    renderer::RenderDevice* device,
    Tilemap2Agent* agent,
    int32_t tilesize,
    const base::Vec2i& atlas_size,
    std::vector<Tilemap2Impl::AtlasCompositeCommand> make_commands) {
  auto* context = device->GetContext();

  agent->atlas_texture.Release();
  renderer::CreateTexture2D(**device, &agent->atlas_texture, "tilemap2.atlas",
                            atlas_size);

  agent->atlas_binding = agent->atlas_texture->GetDefaultView(
      Diligent::TEXTURE_VIEW_SHADER_RESOURCE);

  for (auto& it : make_commands) {
    Diligent::CopyTextureAttribs copy_attribs;
    Diligent::Box box;

    copy_attribs.pSrcTexture = it.texture->data;
    copy_attribs.pSrcBox = &box;
    copy_attribs.SrcTextureTransitionMode =
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

    box.MinX = it.src_rect.x;
    box.MinY = it.src_rect.y;
    box.MaxX = it.src_rect.x + it.src_rect.width;
    box.MaxY = it.src_rect.y + it.src_rect.height;

    if (it.src_rect.x + it.src_rect.width > it.texture->size.x ||
        it.src_rect.y + it.src_rect.height > it.texture->size.y)
      continue;

    copy_attribs.pDstTexture = agent->atlas_texture;
    copy_attribs.DstX = it.dst_pos.x;
    copy_attribs.DstY = it.dst_pos.y;
    copy_attribs.DstTextureTransitionMode =
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

    if (it.dst_pos.x + it.src_rect.width > atlas_size.x ||
        it.dst_pos.y + it.src_rect.height > atlas_size.y)
      continue;

    context->CopyTexture(copy_attribs);
  }

  {
    // Create shadow atlas
    SDL_Surface* shadow_set = CreateShadowSet(tilesize);

    // Update shadow texture
    const base::Rect shadow_region =
        kShadowAtlasArea * base::Rect(tilesize, tilesize, tilesize, tilesize);

    Diligent::Box box(shadow_region.x, shadow_region.x + shadow_region.width,
                      shadow_region.y, shadow_region.y + shadow_region.height);

    Diligent::TextureSubResData sub_res_data;
    sub_res_data.pData = shadow_set->pixels;
    sub_res_data.Stride = shadow_set->pitch;

    context->UpdateTexture(agent->atlas_texture, 0, 0, box, sub_res_data,
                           Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
                           Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Release shadow atlas
    SDL_DestroySurface(shadow_set);
  }
}

void GPUUpdateQuadBatchInternal(renderer::RenderDevice* device,
                                Tilemap2Agent* agent,
                                std::vector<renderer::Quad> ground_cache,
                                std::vector<renderer::Quad> above_cache) {
  agent->ground_draw_count = ground_cache.size();
  agent->above_draw_count = above_cache.size();

  std::vector<renderer::Quad> batch_data;
  batch_data.reserve(ground_cache.size() + above_cache.size());
  batch_data.insert(batch_data.end(),
                    std::make_move_iterator(ground_cache.begin()),
                    std::make_move_iterator(ground_cache.end()));
  batch_data.insert(batch_data.end(),
                    std::make_move_iterator(above_cache.begin()),
                    std::make_move_iterator(above_cache.end()));

  agent->batch->QueueWrite(device->GetContext(), batch_data.data(),
                           batch_data.size());
  device->GetQuadIndex()->Allocate(batch_data.size());
}

void GPUUpdateTilemapUniformInternal(renderer::RenderDevice* device,
                                     Tilemap2Agent* agent,
                                     const base::Vec2& offset,
                                     const base::Vec2& anim_offset,
                                     int32_t tilesize) {
  base::Vec2i atlas_size(agent->atlas_texture->GetDesc().Width,
                         agent->atlas_texture->GetDesc().Height);

  renderer::Binding_Tilemap2::Params uniform;
  uniform.OffsetAndTexSize =
      base::MakeVec4(offset, base::MakeInvert(atlas_size));
  uniform.AnimationOffsetAndTileSize = base::MakeVec4(anim_offset, tilesize);

  device->GetContext()->UpdateBuffer(
      agent->uniform_buffer, 0, sizeof(uniform), &uniform,
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void GPURenderGroundLayerInternal(renderer::RenderDevice* device,
                                  Diligent::IBuffer** world_binding,
                                  Tilemap2Agent* agent) {
  if (!agent->atlas_texture)
    return;

  if (agent->ground_draw_count) {
    auto* context = device->GetContext();
    auto& pipeline_set = device->GetPipelines()->tilemap2;
    auto* pipeline = pipeline_set.GetPipeline(renderer::BlendType::NORMAL);

    // Setup uniform params
    agent->shader_binding->u_transform->Set(*world_binding);
    agent->shader_binding->u_texture->Set(agent->atlas_binding);
    agent->shader_binding->u_params->Set(agent->uniform_buffer);

    // Apply pipeline state
    context->SetPipelineState(pipeline);
    context->CommitShaderResources(
        **agent->shader_binding,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Apply vertex index
    Diligent::IBuffer* const vertex_buffer = **agent->batch;
    context->SetVertexBuffers(
        0, 1, &vertex_buffer, nullptr,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    context->SetIndexBuffer(
        **device->GetQuadIndex(), 0,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Execute render command
    Diligent::DrawIndexedAttribs draw_indexed_attribs;
    draw_indexed_attribs.NumIndices = 6 * agent->ground_draw_count;
    draw_indexed_attribs.IndexType = device->GetQuadIndex()->format();
    context->DrawIndexed(draw_indexed_attribs);
  }
}

void GPURenderAboveLayerInternal(renderer::RenderDevice* device,
                                 Diligent::IBuffer** world_binding,
                                 Tilemap2Agent* agent) {
  if (!agent->atlas_texture)
    return;

  if (agent->above_draw_count) {
    auto* context = device->GetContext();
    auto& pipeline_set = device->GetPipelines()->tilemap2;
    auto* pipeline = pipeline_set.GetPipeline(renderer::BlendType::NORMAL);

    // Setup uniform params
    agent->shader_binding->u_transform->Set(*world_binding);
    agent->shader_binding->u_texture->Set(agent->atlas_binding);
    agent->shader_binding->u_params->Set(agent->uniform_buffer);

    // Apply pipeline state
    context->SetPipelineState(pipeline);
    context->CommitShaderResources(
        **agent->shader_binding,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Apply vertex index
    Diligent::IBuffer* const vertex_buffer = **agent->batch;
    context->SetVertexBuffers(
        0, 1, &vertex_buffer, nullptr,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    context->SetIndexBuffer(
        **device->GetQuadIndex(), 0,
        Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Execute render command
    Diligent::DrawIndexedAttribs draw_indexed_attribs;
    draw_indexed_attribs.NumIndices = 6 * agent->above_draw_count;
    draw_indexed_attribs.IndexType = device->GetQuadIndex()->format();
    draw_indexed_attribs.FirstIndexLocation = agent->ground_draw_count * 6;
    context->DrawIndexed(draw_indexed_attribs);
  }
}

}  // namespace

TilemapBitmapImpl::TilemapBitmapImpl(base::WeakPtr<Tilemap2Impl> tilemap)
    : tilemap_(tilemap) {}

TilemapBitmapImpl::~TilemapBitmapImpl() = default;

scoped_refptr<Bitmap> TilemapBitmapImpl::Get(int32_t index,
                                             ExceptionState& exception_state) {
  if (!tilemap_)
    return nullptr;

  auto& bitmaps = tilemap_->bitmaps_;
  if (index < 0 || index >= static_cast<int32_t>(bitmaps.size())) {
    exception_state.ThrowError(ExceptionCode::CONTENT_ERROR,
                               "Out range of bitmaps.");
    return nullptr;
  }

  return bitmaps[index].bitmap;
}

void TilemapBitmapImpl::Put(int32_t index,
                            scoped_refptr<Bitmap> texture,
                            ExceptionState& exception_state) {
  if (!tilemap_)
    return;

  auto& bitmaps = tilemap_->bitmaps_;
  if (index < 0 || index >= static_cast<int32_t>(bitmaps.size()))
    return exception_state.ThrowError(ExceptionCode::CONTENT_ERROR,
                                      "Out range of bitmaps.");

  bitmaps[index].bitmap = CanvasImpl::FromBitmap(texture);
  bitmaps[index].observer = bitmaps[index].bitmap->AddCanvasObserver(
      base::BindRepeating(&Tilemap2Impl::AtlasModifyHandlerInternal, tilemap_));
  tilemap_->atlas_dirty_ = true;
}

scoped_refptr<Tilemap2> Tilemap2::New(ExecutionContext* execution_context,
                                      scoped_refptr<Viewport> viewport,
                                      int32_t tilesize,
                                      ExceptionState& exception_state) {
  return new Tilemap2Impl(execution_context->graphics,
                          ViewportImpl::From(viewport), tilesize);
}

Tilemap2Impl::Tilemap2Impl(RenderScreenImpl* screen,
                           scoped_refptr<ViewportImpl> parent,
                           int32_t tilesize)
    : GraphicsChild(screen),
      Disposable(screen),
      ground_node_(parent ? parent->GetDrawableController()
                          : screen->GetDrawableController(),
                   SortKey()),
      above_node_(parent ? parent->GetDrawableController()
                         : screen->GetDrawableController(),
                  SortKey(200)),
      tilesize_(tilesize),
      viewport_(parent) {
  ground_node_.RegisterEventHandler(base::BindRepeating(
      &Tilemap2Impl::GroundNodeHandlerInternal, base::Unretained(this)));
  above_node_.RegisterEventHandler(base::BindRepeating(
      &Tilemap2Impl::AboveNodeHandlerInternal, base::Unretained(this)));

  agent_ = new Tilemap2Agent;
  screen->PostTask(
      base::BindOnce(&GPUCreateTilemapInternal, screen->GetDevice(), agent_));
}

Tilemap2Impl::~Tilemap2Impl() {
  ExceptionState exception_state;
  Dispose(exception_state);
}

void Tilemap2Impl::SetLabel(const std::string& label,
                            ExceptionState& exception_state) {
  ground_node_.SetDebugLabel(label);
  above_node_.SetDebugLabel(label);
}

void Tilemap2Impl::Dispose(ExceptionState& exception_state) {
  Disposable::Dispose(exception_state);
}

bool Tilemap2Impl::IsDisposed(ExceptionState& exception_state) {
  return Disposable::IsDisposed(exception_state);
}

void Tilemap2Impl::Update(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  if (++frame_index_ >= 30 * 3 * 4)
    frame_index_ = 0;

  const uint8_t kAniIndicesRegular[3 * 4] = {0, 1, 2, 1, 0, 1,
                                             2, 1, 0, 1, 2, 1};
  const uint8_t kAniIndicesWaterfall[3 * 4] = {0, 1, 2, 0, 1, 2,
                                               0, 1, 2, 0, 1, 2};

  const uint8_t animation_index1 = kAniIndicesRegular[frame_index_ / 30];
  const uint8_t animation_index2 = kAniIndicesWaterfall[frame_index_ / 30];
  animation_offset_ = base::Vec2(animation_index1 * 2 * tilesize_,
                                 animation_index2 * tilesize_);

  flash_timer_ = ++flash_timer_ % 32;
  flash_opacity_ = std::abs(16 - flash_timer_) * 8 + 32;
  if (flash_count_)
    map_buffer_dirty_ = true;
}

scoped_refptr<TilemapBitmap> Tilemap2Impl::Bitmaps(
    ExceptionState& exception_state) {
  return new TilemapBitmapImpl(weak_ptr_factory_.GetWeakPtr());
}

scoped_refptr<Table> Tilemap2Impl::Get_MapData(
    ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return map_data_;
}

void Tilemap2Impl::Put_MapData(const scoped_refptr<Table>& value,
                               ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  map_data_ = TableImpl::From(value);
  map_data_observer_ = map_data_->AddObserver(base::BindRepeating(
      &Tilemap2Impl::MapDataModifyHandlerInternal, base::Unretained(this)));
  map_buffer_dirty_ = true;
}

scoped_refptr<Table> Tilemap2Impl::Get_FlashData(
    ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return flash_data_;
}

void Tilemap2Impl::Put_FlashData(const scoped_refptr<Table>& value,
                                 ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  flash_data_ = TableImpl::From(value);
  flash_data_observer_ = flash_data_->AddObserver(base::BindRepeating(
      &Tilemap2Impl::MapDataModifyHandlerInternal, base::Unretained(this)));
  map_buffer_dirty_ = true;
}

scoped_refptr<Table> Tilemap2Impl::Get_Passages(
    ExceptionState& exception_state) {
  return Get_Flags(exception_state);
}

void Tilemap2Impl::Put_Passages(const scoped_refptr<Table>& value,
                                ExceptionState& exception_state) {
  Put_Flags(value, exception_state);
}

scoped_refptr<Table> Tilemap2Impl::Get_Flags(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  return flags_;
}

void Tilemap2Impl::Put_Flags(const scoped_refptr<Table>& value,
                             ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CHECK_ATTRIBUTE_VALUE;

  flags_ = TableImpl::From(value);
  flags_observer_ = flags_->AddObserver(base::BindRepeating(
      &Tilemap2Impl::MapDataModifyHandlerInternal, base::Unretained(this)));
  map_buffer_dirty_ = true;
}

scoped_refptr<Viewport> Tilemap2Impl::Get_Viewport(
    ExceptionState& exception_state) {
  return viewport_;
}

void Tilemap2Impl::Put_Viewport(const scoped_refptr<Viewport>& value,
                                ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  if (viewport_ == value)
    return;

  viewport_ = ViewportImpl::From(value);
  DrawNodeController* controller = viewport_
                                       ? viewport_->GetDrawableController()
                                       : screen()->GetDrawableController();
  ground_node_.RebindController(controller);
  above_node_.RebindController(controller);
}

bool Tilemap2Impl::Get_Visible(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return false;

  return ground_node_.GetVisibility();
}

void Tilemap2Impl::Put_Visible(const bool& value,
                               ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  ground_node_.SetNodeVisibility(value);
  above_node_.SetNodeVisibility(value);
}

int32_t Tilemap2Impl::Get_Ox(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return origin_.x;
}

void Tilemap2Impl::Put_Ox(const int32_t& value,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  origin_.x = value;
}

int32_t Tilemap2Impl::Get_Oy(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;

  return origin_.y;
}

void Tilemap2Impl::Put_Oy(const int32_t& value,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  origin_.y = value;
}

void Tilemap2Impl::OnObjectDisposed() {
  ground_node_.DisposeNode();
  above_node_.DisposeNode();

  screen()->PostTask(base::BindOnce(&GPUDestroyTilemapInternal, agent_));
  agent_ = nullptr;
}

void Tilemap2Impl::GroundNodeHandlerInternal(
    DrawableNode::RenderStage stage,
    DrawableNode::RenderControllerParams* params) {
  if (stage == DrawableNode::RenderStage::BEFORE_RENDER) {
    UpdateViewportInternal(params->viewport, params->origin);

    if (atlas_dirty_) {
      std::vector<AtlasCompositeCommand> commands;
      base::Vec2i atlas_size = MakeAtlasInternal(commands);

      screen()->PostTask(base::BindRepeating(&GPUMakeAtlasInternal,
                                             params->device, agent_, tilesize_,
                                             atlas_size, std::move(commands)));

      atlas_dirty_ = false;
    }

    if (map_buffer_dirty_) {
      std::vector<renderer::Quad> ground_cache;
      std::vector<renderer::Quad> above_cache;
      ParseMapDataInternal(ground_cache, above_cache);

      screen()->PostTask(base::BindRepeating(
          &GPUUpdateQuadBatchInternal, params->device, agent_,
          std::move(ground_cache), std::move(above_cache)));

      map_buffer_dirty_ = false;
    }

    screen()->PostTask(base::BindRepeating(
        &GPUUpdateTilemapUniformInternal, params->device, agent_,
        render_offset_, animation_offset_, tilesize_));
  } else if (stage == DrawableNode::RenderStage::ON_RENDERING) {
    screen()->PostTask(base::BindRepeating(&GPURenderGroundLayerInternal,
                                           params->device,
                                           params->world_binding, agent_));
  }
}

void Tilemap2Impl::AboveNodeHandlerInternal(
    DrawableNode::RenderStage stage,
    DrawableNode::RenderControllerParams* params) {
  if (stage == DrawableNode::RenderStage::ON_RENDERING) {
    screen()->PostTask(base::BindRepeating(&GPURenderAboveLayerInternal,
                                           params->device,
                                           params->world_binding, agent_));
  }
}

void Tilemap2Impl::UpdateViewportInternal(const base::Rect& viewport,
                                          const base::Vec2i& viewport_origin) {
  const base::Vec2i tilemap_origin = origin_ + viewport_origin;
  const base::Vec2i viewport_size = viewport.Size();

  base::Rect new_viewport;
  new_viewport.x = tilemap_origin.x / tilesize_;
  new_viewport.y = tilemap_origin.y / tilesize_ - 1;
  new_viewport.width =
      (viewport_size.x / tilesize_) + !!(viewport_size.x % tilesize_) + 1;
  new_viewport.height =
      (viewport_size.y / tilesize_) + !!(viewport_size.y % tilesize_) + 2;

  const base::Vec2i display_offset = tilemap_origin % base::Vec2i(tilesize_);
  render_offset_ = -display_offset - base::Vec2i(0, tilesize_);

  if (!(new_viewport == render_viewport_)) {
    render_viewport_ = new_viewport;
    map_buffer_dirty_ = true;
  }
}

base::Vec2i Tilemap2Impl::MakeAtlasInternal(
    std::vector<AtlasCompositeCommand>& commands) {
  for (size_t i = 0; i < _countof(kTilemapAtlas); ++i) {
    auto& atlas_info = kTilemapAtlas[i];
    scoped_refptr atlas_bitmap = bitmaps_[atlas_info.tile_id].bitmap;

    if (!atlas_bitmap || !atlas_bitmap->GetAgent())
      continue;

    base::Rect src_rect(atlas_info.src_rect.x * tilesize_,
                        atlas_info.src_rect.y * tilesize_,
                        atlas_info.src_rect.width * tilesize_,
                        atlas_info.src_rect.height * tilesize_);
    base::Vec2i dst_pos(atlas_info.dest_pos.x * tilesize_,
                        atlas_info.dest_pos.y * tilesize_);

    AtlasCompositeCommand command;
    command.src_rect = src_rect;
    command.dst_pos = dst_pos;
    command.texture = atlas_bitmap->GetAgent();
    commands.push_back(std::move(command));
  }

  return base::Vec2i(tilesize_ * 64, tilesize_ * 32);
}

// Reference:
//  https://web.archive.org/web/20230925131126/https://www.tktkgame.com/tkool/memo/vx/tile_id.html
//  - RPGMV Tilemap
void Tilemap2Impl::ParseMapDataInternal(
    std::vector<renderer::Quad>& ground_cache,
    std::vector<renderer::Quad>& above_cache) {
  auto value_wrap = [&](int32_t value, int32_t range) {
    int32_t res = value % range;
    return res < 0 ? res + range : res;
  };

  auto get_wrap_data = [&](scoped_refptr<TableImpl> t, int32_t x, int32_t y,
                           int32_t z) -> int16_t {
    if (!t)
      return 0;

    return t->value(value_wrap(x, t->x_size()), value_wrap(y, t->y_size()), z);
  };

  auto get_map_flag = [&](scoped_refptr<TableImpl> t, int32_t x) -> int16_t {
    if (!t)
      return 0;

    if (x < 0 || x >= static_cast<int32_t>(t->x_size()))
      return 0;

    return t->value(x, 0, 0);
  };

  auto process_quads = [&](renderer::Quad* quads, int32_t size, bool above) {
    std::vector<renderer::Quad>* target = above ? &above_cache : &ground_cache;
    target->insert(target->end(), quads, quads + size);
  };

  auto autotile_set_pos = [&](base::RectF& pos, int32_t i) {
    switch (i) {
      case 0:  // Left Top
        break;
      case 1:  // Right Top
        pos.x += tilesize_ / 2.0f;
        break;
      case 2:  // Left Bottom
        pos.y += tilesize_ / 2.0f;
        break;
      case 3:  // Right bottom
        pos.x += tilesize_ / 2.0f;
        pos.y += tilesize_ / 2.0f;
        break;
      case 4:  // Table's Left Bottom
        pos.y += tilesize_ * 0.75f;
        break;
      case 5:  // Table's Right Bottom
        pos.x += tilesize_ / 2.0f;
        pos.y += tilesize_ * 0.75f;
        break;
      default:
        break;
    }
  };

  auto read_autotile_common = [&](int32_t pattern_id, const base::Vec2i& offset,
                                  const base::Vec4& color, int32_t x, int32_t y,
                                  const base::RectF* rect_src, bool above) {
    renderer::Quad quads[4];

    for (int32_t i = 0; i < 4; ++i) {
      base::RectF tex_rect =
          rect_src[pattern_id * 4 + i] *
          base::RectF(tilesize_, tilesize_, tilesize_, tilesize_);
      tex_rect.x += offset.x * tilesize_ + 0.5f;
      tex_rect.y += offset.y * tilesize_ + 0.5f;
      tex_rect.width -= 1.0f;
      tex_rect.height -= 1.0f;

      base::RectF pos_rect(x * tilesize_, y * tilesize_, tilesize_ / 2.0f,
                           tilesize_ / 2.0f);
      autotile_set_pos(pos_rect, i);

      renderer::Quad::SetTexCoordRectNorm(&quads[i], tex_rect);
      renderer::Quad::SetPositionRect(&quads[i], pos_rect);
      renderer::Quad::SetColor(&quads[i], color);
    }

    process_quads(quads, 4, above);
  };

  auto read_autotile_table = [&](int32_t pattern_id, const base::Vec2i& offset,
                                 const base::Vec4& color, int32_t x, int32_t y,
                                 bool occlusion, bool above) {
    renderer::Quad quads[6];

    for (int32_t i = 0; i < 6; ++i) {
      const base::RectF tile_src = kAutotileSrcTable[pattern_id * 6 + i];
      base::RectF tex_rect =
          tile_src * base::RectF(tilesize_, tilesize_, tilesize_, tilesize_);
      tex_rect.x += offset.x * tilesize_ + 0.5f;
      tex_rect.y += offset.y * tilesize_ + 0.5f;
      tex_rect.width = std::max(0.0f, tex_rect.width - 1.0f);
      tex_rect.height = std::max(0.0f, tex_rect.height - 1.0f);

      base::RectF pos_rect(x * tilesize_, y * tilesize_,
                           tile_src.width * tilesize_,
                           tile_src.height * tilesize_);
      autotile_set_pos(pos_rect, i);

      if (occlusion && i >= 4) {
        const float table_leg = tilesize_ * 0.25f;
        tex_rect.height -= table_leg;
        pos_rect.height -= table_leg;
      }

      renderer::Quad::SetTexCoordRectNorm(&quads[i], tex_rect);
      renderer::Quad::SetPositionRect(&quads[i], pos_rect);
      renderer::Quad::SetColor(&quads[i], color);
    }

    process_quads(quads, 6, above);
  };

  auto read_autotile_waterfall =
      [&](int32_t pattern_id, const base::Vec2i& offset,
          const base::Vec4& color, int32_t x, int32_t y, bool above) {
        if (pattern_id > 0x3)
          return;

        renderer::Quad quads[2];

        for (size_t i = 0; i < 2; ++i) {
          base::RectF tex_rect =
              kAutotileSrcWaterfall[pattern_id * 2 + i] *
              base::RectF(tilesize_, tilesize_, tilesize_, tilesize_);
          tex_rect.x += offset.x * tilesize_ + 0.5f;
          tex_rect.y += offset.y * tilesize_ + 0.5f;
          tex_rect.width -= 1;
          tex_rect.height -= 1;

          base::RectF pos_rect(x * tilesize_ + i * (tilesize_ / 2.0f),
                               y * tilesize_, tilesize_ / 2.0f, tilesize_);

          renderer::Quad::SetTexCoordRectNorm(&quads[i], tex_rect);
          renderer::Quad::SetPositionRect(&quads[i], pos_rect);
          renderer::Quad::SetColor(&quads[i], color);
        }

        process_quads(quads, 2, above);
      };

  auto process_tile_A1 = [&](int16_t tile_id, const base::Vec4& color,
                             int32_t x, int32_t y, bool above) {
    tile_id -= 0x0800;

    const int32_t autotile_id = tile_id / 0x30;
    const int32_t pattern_id = tile_id % 0x30;

    // clang-format off
    const base::Vec2i waterfall(-1, -1);
    const base::Vec2i src_offset[] = {
        {0,  0},  {0,  3},
        {12, 0},  {12, 3},
        {6,  0},  waterfall,
        {6,  3},  waterfall,

        {0,  6},  waterfall,
        {0,  9},  waterfall,
        {6,  6},  waterfall,
        {6,  9},  waterfall};
    const base::Vec2i waterfall_offset[] = {
        {14, 0}, {14, 3},
        {12, 6}, {12, 9},
        {14, 6}, {14, 9},
    };
    // clang-format on

    // Transform pattern source to waterfall style
    const base::Vec2i src_pos = src_offset[autotile_id];
    if (src_pos.x == -1)
      return read_autotile_waterfall(pattern_id,
                                     waterfall_offset[(autotile_id - 5) / 2],
                                     color, x, y, above);

    read_autotile_common(pattern_id, src_pos, color, x, y, kAutotileSrcRegular,
                         above);
  };

  auto process_tile_A2 = [&](int16_t tile_id, const base::Vec4& color,
                             int32_t x, int32_t y, bool above, bool is_table,
                             bool occlusion) {
    tile_id -= 0x0B00;

    const int32_t autotile_id = tile_id / 0x30;
    const int32_t pattern_id = tile_id % 0x30;

    // Process table foot occlusion
    base::Vec2i offset(16 + (autotile_id % 8) * 2, (autotile_id / 8) * 3);
    if (is_table)
      return read_autotile_table(pattern_id, offset, color, x, y, occlusion,
                                 above);
    read_autotile_common(pattern_id, offset, color, x, y, kAutotileSrcRegular,
                         above);
  };

  auto process_tile_A3 = [&](int16_t tile_id, const base::Vec4& color,
                             int32_t x, int32_t y, bool above) {
    tile_id -= 0x1100;

    const int32_t autotile_id = tile_id / 0x30;
    const int32_t pattern_id = tile_id % 0x30;
    if (pattern_id >= 0x10)
      return;

    const base::Vec2i offset((autotile_id % 8) * 2, (autotile_id / 8) * 2 + 12);
    read_autotile_common(pattern_id, offset, color, x, y, kAutotileSrcWall,
                         above);
  };

  auto process_tile_A4 = [&](int16_t tile_id, const base::Vec4& color,
                             int32_t x, int32_t y, bool above) {
    tile_id -= 0x1700;

    const int32_t autotile_id = tile_id / 0x30;
    const int32_t pattern_id = tile_id % 0x30;

    const int32_t vertical_offset[] = {0, 3, 5, 8, 10, 13};
    const int32_t offset_index = autotile_id / 8;
    const base::Vec2i offset(16 + (autotile_id % 8) * 2,
                             12 + vertical_offset[offset_index]);

    if (!(offset_index % 2)) {
      read_autotile_common(pattern_id, offset, color, x, y, kAutotileSrcRegular,
                           above);
    } else {
      if (pattern_id >= 0x10)
        return;

      read_autotile_common(pattern_id, offset, color, x, y, kAutotileSrcWall,
                           above);
    }
  };

  auto process_tile_A5 = [&](int16_t tile_id, const base::Vec4& color,
                             int32_t x, int32_t y, bool above) {
    tile_id -= 0x0600;

    int32_t ox = tile_id % 0x8;
    int32_t oy = tile_id / 0x8;

    if (oy >= 8) {
      oy -= 8;
      ox += 8;
    }

    const base::Vec2 atlas_position(ox * tilesize_ + 0.5f,
                                    (20 + oy) * tilesize_ + 0.5f);

    base::RectF tex(atlas_position, base::Vec2(tilesize_ - 1.0f));
    base::RectF pos(base::Vec2(x * tilesize_, y * tilesize_),
                    base::Vec2(tilesize_));

    renderer::Quad quad;
    renderer::Quad::SetTexCoordRectNorm(&quad, tex);
    renderer::Quad::SetPositionRect(&quad, pos);
    renderer::Quad::SetColor(&quad, color);

    process_quads(&quad, 1, above);
  };

  auto process_tile_bcde = [&](int16_t tile_id, const base::Vec4& color,
                               int32_t x, int32_t y, bool above) {
    int32_t ox = tile_id % 0x8;
    int32_t oy = (tile_id / 0x8) % 0x10;
    int32_t ob = tile_id / (0x8 * 0x10);

    ox += (ob % 2) * 0x8;
    oy += (ob / 2) * 0x10;

    if (oy >= 48) {
      /* E atlas */
      oy -= 32;
      ox += 16;
    } else if (oy >= 32) {
      /* D atlas */
      oy -= 16;
    } else if (oy >= 16) {
      /* C atlas */
      oy -= 16;
      ox += 16;
    }

    const base::Vec2 atlas_position((32 + ox) * tilesize_ + 0.5f,
                                    oy * tilesize_ + 0.5f);

    base::RectF tex(atlas_position, base::Vec2(tilesize_ - 1.0f));
    base::RectF pos(base::Vec2(x * tilesize_, y * tilesize_),
                    base::Vec2(tilesize_));

    renderer::Quad quad;
    renderer::Quad::SetTexCoordRectNorm(&quad, tex);
    renderer::Quad::SetPositionRect(&quad, pos);
    renderer::Quad::SetColor(&quad, color);

    process_quads(&quad, 1, above);
  };

  auto process_shadow_tile = [&](int8_t shadow_id, int32_t x, int32_t y) {
    int32_t ox = shadow_id;

    const base::Vec2 atlas_position(
        (kShadowAtlasArea.x + ox) * tilesize_ + 0.5f,
        (kShadowAtlasArea.y) * tilesize_ + 0.5f);

    base::RectF tex(atlas_position, base::Vec2(tilesize_ - 1.0f));
    base::RectF pos(base::Vec2(x * tilesize_, y * tilesize_),
                    base::Vec2(tilesize_));

    renderer::Quad quad;
    renderer::Quad::SetTexCoordRectNorm(&quad, tex);
    renderer::Quad::SetPositionRect(&quad, pos);
    renderer::Quad::SetColor(&quad, base::Vec4(0));

    process_quads(&quad, 1, false);
  };

  auto process_common_tile = [&](int16_t tile_id, const base::Vec4& color,
                                 int32_t x, int32_t y, int32_t z,
                                 int16_t under_tile_id) {
    int16_t flag = get_map_flag(flags_, tile_id);
    bool over_player = (flag & 0x10) && (z >= 2);
    bool is_table =
        (screen()->GetAPIVersion() >= ContentProfile::APIVersion::RGSS3)
            ? (flag & 0x80)
            : (tile_id - 0x0B00) % (8 * 0x30) >= (7 * 0x30);

    if (tile_id >= 0x0800 && tile_id < 0x0B00)  // A1
      return process_tile_A1(tile_id, color, x, y, over_player);
    if (tile_id >= 0x0B00 && tile_id < 0x1100)  // A2
      return process_tile_A2(tile_id, color, x, y, over_player, is_table,
                             under_tile_id >= 0x1700 && under_tile_id < 0x2000);
    if (tile_id >= 0x1100 && tile_id < 0x1700)  // A3
      return process_tile_A3(tile_id, color, x, y, over_player);
    if (tile_id >= 0x1700 && tile_id < 0x2000)  // A4
      return process_tile_A4(tile_id, color, x, y, over_player);
    if (tile_id >= 0x0600 && tile_id < 0x0680)  // A5
      return process_tile_A5(tile_id, color, x, y, over_player);
    if (tile_id < 0x0400)  // B ~ E
      return process_tile_bcde(tile_id, color, x, y, over_player);
  };

  auto process_shadow_layer = [&](int32_t ox, int32_t oy, int32_t w,
                                  int32_t h) {
    for (int32_t y = 0; y < h; ++y)
      for (int32_t x = 0; x < w; ++x)
        process_shadow_tile(get_wrap_data(map_data_, x + ox, y + oy, 3), x, y);
  };

  auto process_common_layer = [&](int32_t ox, int32_t oy, int32_t w, int32_t h,
                                  int32_t z) {
    for (int32_t y = h - 1; y >= 0; --y) {
      for (int32_t x = 0; x < w; ++x) {
        // Common tile id
        const int16_t tile_id = get_wrap_data(map_data_, x + ox, y + oy, z);
        if (!tile_id)
          continue;

        // For table foot occlusion
        const int16_t under_tile_id =
            get_wrap_data(map_data_, x + ox, y + oy + 1, 0);

        // Flash data
        const int16_t flash_color =
            get_wrap_data(flash_data_, x + ox, y + oy, 0);

        // Process tile flash
        base::Vec4 blend_color;
        if (flash_color) {
          const float blue = ((flash_color & 0x000F) >> 0) / 0xF;
          const float green = ((flash_color & 0x00F0) >> 4) / 0xF;
          const float red = ((flash_color & 0x0F00) >> 8) / 0xF;
          blend_color = base::Vec4(red, green, blue, flash_opacity_ / 255.0f);

          ++flash_count_;
        }

        // Process tile (non-shadow tile)
        process_common_tile(tile_id, blend_color, x, y, z, under_tile_id);
      }
    }
  };

  auto read_tilemap = [&](const base::Rect& viewport) {
    int32_t ox = viewport.x, oy = viewport.y;
    int32_t w = viewport.width, h = viewport.height;

    // A aera (0 - 1)
    process_common_layer(ox, oy, w, h, 0);
    process_common_layer(ox, oy, w, h, 1);

    // Shadow area (3)
    if (screen()->GetAPIVersion() == ContentProfile::APIVersion::RGSS3)
      process_shadow_layer(ox, oy, w, h);

    // BCDE area (2)
    process_common_layer(ox, oy, w, h, 2);
  };

  // Reset flash quads
  flash_count_ = 0;

  // Process tilemap data
  read_tilemap(render_viewport_);
}

void Tilemap2Impl::AtlasModifyHandlerInternal() {
  atlas_dirty_ = true;
}

void Tilemap2Impl::MapDataModifyHandlerInternal() {
  map_buffer_dirty_ = true;
}

}  // namespace content
