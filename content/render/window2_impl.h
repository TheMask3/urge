// Copyright 2018-2025 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDER_WINDOW2_IMPL_H_
#define CONTENT_RENDER_WINDOW2_IMPL_H_

#include "content/canvas/canvas_impl.h"
#include "content/common/color_impl.h"
#include "content/common/rect_impl.h"
#include "content/common/tone_impl.h"
#include "content/context/disposable.h"
#include "content/public/engine_window2.h"
#include "content/render/drawable_controller.h"
#include "content/screen/viewport_impl.h"
#include "renderer/device/render_device.h"

namespace content {

struct Window2Agent {
  std::unique_ptr<renderer::QuadBatch> background_batch;
  std::unique_ptr<renderer::QuadBatch> controls_batch;

  std::unique_ptr<renderer::Binding_Base> shader_binding;

  RRefPtr<Diligent::ITexture> background_texture;
  RRefPtr<Diligent::IBuffer> background_world;
  RRefPtr<Diligent::IBuffer> uniform_buffer;

  int32_t background_draw_count;
  int32_t controls_draw_count;
  int32_t cursor_draw_count;
  int32_t contents_draw_count;
};

class Window2Impl : public Window2, public GraphicsChild, public Disposable {
 public:
  Window2Impl(RenderScreenImpl* screen,
              scoped_refptr<ViewportImpl> parent,
              const base::Rect& bound,
              int32_t scale);
  ~Window2Impl() override;

  Window2Impl(const Window2Impl&) = delete;
  Window2Impl& operator=(const Window2Impl&) = delete;

  void SetLabel(const std::string& label,
                ExceptionState& exception_state) override;

  void Dispose(ExceptionState& exception_state) override;
  bool IsDisposed(ExceptionState& exception_state) override;
  void Update(ExceptionState& exception_state) override;
  void Move(int32_t x,
            int32_t y,
            int32_t width,
            int32_t height,
            ExceptionState& exception_state) override;
  bool IsOpened(ExceptionState& exception_state) override;
  bool IsClosed(ExceptionState& exception_state) override;

  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Viewport, scoped_refptr<Viewport>);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Windowskin, scoped_refptr<Bitmap>);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Contents, scoped_refptr<Bitmap>);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(CursorRect, scoped_refptr<Rect>);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Active, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Visible, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(ArrowsVisible, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Pause, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(X, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Y, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Width, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Height, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Z, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Ox, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Oy, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Padding, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(PaddingBottom, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Opacity, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(BackOpacity, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(ContentsOpacity, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Openness, int32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Tone, scoped_refptr<Tone>);

 private:
  void OnObjectDisposed() override;
  std::string DisposedObjectName() override { return "Window2"; }
  void DrawableNodeHandlerInternal(
      DrawableNode::RenderStage stage,
      DrawableNode::RenderControllerParams* params);
  void BackgroundTextureObserverInternal();

  bool rgss3_style_ = false;
  DrawableNode node_;
  Window2Agent* agent_;
  int32_t scale_ = 2;
  int32_t pause_index_ = 0;
  int32_t cursor_opacity_ = 255;
  bool cursor_fade_ = false;
  bool background_dirty_ = false;

  scoped_refptr<ViewportImpl> viewport_;
  scoped_refptr<CanvasImpl> windowskin_;
  scoped_refptr<CanvasImpl> contents_;
  scoped_refptr<RectImpl> cursor_rect_;

  bool active_ = true;
  bool arrows_visible_ = true;
  bool pause_ = false;

  base::Rect bound_;
  base::Vec2i origin_;

  int32_t padding_ = 12;
  int32_t padding_bottom_ = 12;

  int32_t opacity_ = 255;
  int32_t back_opacity_ = 255;
  int32_t contents_opacity_ = 255;
  int32_t openness_ = 255;

  scoped_refptr<ToneImpl> tone_;
};

}  // namespace content

#endif  //! CONTENT_RENDER_WINDOW2_IMPL_H_
