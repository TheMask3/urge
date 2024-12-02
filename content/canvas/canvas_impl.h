// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CANVAS_CANVAS_IMPL_H_
#define CONTENT_CANVAS_CANVAS_IMPL_H_

#include "SDL3/SDL_surface.h"

#include "base/containers/linked_list.h"
#include "content/public/engine_bitmap.h"
#include "content/render/drawable_controller.h"
#include "renderer/context/device_context.h"

#include <queue>

namespace content {

class CanvasScheduler;

class CanvasImpl : public Bitmap, public base::LinkNode<CanvasImpl> {
 public:
  CanvasImpl(CanvasScheduler* scheduler);
  ~CanvasImpl() override;

  CanvasImpl(const CanvasImpl&) = delete;
  CanvasImpl& operator=(const CanvasImpl&) = delete;

  // Synchronize pending commands and fetch texture to buffer.
  // Read buffer for surface pixels data.
  SDL_Surface* FetchSurfacePixels();

  // Process queued pending commands.
  void SubmitQueuedCommands(const wgpu::CommandEncoder& encoder);

 protected:
  void Dispose() override;
  bool IsDisposed() override;
  uint32_t Width() override;
  uint32_t Height() override;
  scoped_refptr<Rect> GetRect() override;
  void Blt(int32_t x,
           int32_t y,
           scoped_refptr<Bitmap> src_bitmap,
           scoped_refptr<Rect> src_rect,
           uint32_t opacity) override;
  void StretchBlt(scoped_refptr<Rect> dest_rect,
                  scoped_refptr<Bitmap> src_bitmap,
                  scoped_refptr<Rect> src_rect,
                  uint32_t opacity) override;
  void FillRect(int32_t x,
                int32_t y,
                uint32_t width,
                uint32_t height,
                scoped_refptr<Color> color) override;
  void FillRect(scoped_refptr<Rect> rect, scoped_refptr<Color> color) override;
  void GradientFillRect(int32_t x,
                        int32_t y,
                        uint32_t width,
                        uint32_t height,
                        scoped_refptr<Color> color1,
                        scoped_refptr<Color> color2,
                        bool vertical) override;
  void GradientFillRect(scoped_refptr<Rect> rect,
                        scoped_refptr<Color> color1,
                        scoped_refptr<Color> color2,
                        bool vertical) override;
  void Clear() override;
  void ClearRect(int32_t x,
                 int32_t y,
                 uint32_t width,
                 uint32_t height) override;
  void ClearRect(scoped_refptr<Rect> rect) override;
  scoped_refptr<Color> GetPixel(int32_t x, int32_t y) override;
  void SetPixel(int32_t x, int32_t y, scoped_refptr<Color> color) override;
  void HueChange(int32_t hue) override;
  void Blur() override;
  void RadialBlur(int32_t angle, int32_t division) override;
  void DrawText(int32_t x,
                int32_t y,
                uint32_t width,
                uint32_t height,
                const std::string& str,
                int32_t align) override;
  void DrawText(scoped_refptr<Rect> rect,
                const std::string& str,
                int32_t align) override;
  scoped_refptr<Rect> TextSize(const std::string& str) override;

  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Font, scoped_refptr<Font>);

 private:
  enum class DrawCommandID {
    kFillRect = 0,
    kGradientFillRect,
    kHueChange,
    kBlur,
    kRadialBlur,
    kDrawText,
  };

  union alignas(32) CanvasDrawCommand {
    DrawCommandID command_id;
    struct {
      DrawCommandID reserved;
      base::Rect region;
      base::Vec4 color;
    } fill_rect;

    struct {
      DrawCommandID reserved;
      base::Rect region;
      base::Vec4 color1;
      base::Vec4 color2;
      bool vertical;
    } gradient_fill_rect;

    struct {
      DrawCommandID reserved;
      int32_t hue;
    } hue_change;

    struct {
      DrawCommandID reserved;
      int32_t angle;
      int32_t division;
    } radial_blur;

    struct {
      DrawCommandID reserved;
      std::string text;
      base::Rect region;
      int32_t text_align;
    } draw_text;

    CanvasDrawCommand() {}
    ~CanvasDrawCommand() {}
  };

  CanvasDrawCommand* AllocateCommand();

  CanvasScheduler* scheduler_;
  wgpu::Texture canvas_target_;
  std::queue<CanvasDrawCommand> queued_;

  scoped_refptr<Font> font_;
};

}  // namespace content

#endif  //! CONTENT_CANVAS_CANVAS_IMPL_H_
