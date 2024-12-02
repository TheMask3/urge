// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/canvas/canvas_impl.h"

#include "SDL3_ttf/SDL_ttf.h"

#include "content/canvas/canvas_scheduler.h"
#include "content/common/color_impl.h"
#include "content/common/rect_impl.h"

namespace content {

scoped_refptr<Bitmap> Bitmap::New(const std::string& filename) {
  return scoped_refptr<Bitmap>();
}

scoped_refptr<Bitmap> Bitmap::New(uint32_t width, uint32_t height) {
  return scoped_refptr<Bitmap>();
}

CanvasImpl::CanvasImpl(CanvasScheduler* scheduler) : scheduler_(scheduler) {}

CanvasImpl::~CanvasImpl() {
  Dispose();
}

SDL_Surface* CanvasImpl::FetchSurfacePixels() {
  // Submit pending commands
  auto* encoder = scheduler_->GetDrawContext()->GetImmediateEncoder();
  SubmitQueuedCommands(*encoder);

  // Synchronize immediate context and flush
  scheduler_->GetDrawContext()->Flush();

  // Fetch buffer to texture

  return nullptr;
}

void CanvasImpl::SubmitQueuedCommands(const wgpu::CommandEncoder& encoder) {
  auto& current_command = queued_.front();
  switch (current_command.command_id) {
    default:
      break;
  }

  queued_.pop();
}

void CanvasImpl::Dispose() {
  RemoveFromList();
  canvas_target_ = nullptr;
}

bool CanvasImpl::IsDisposed() {
  return !!canvas_target_;
}

uint32_t CanvasImpl::Width() {
  return canvas_target_.GetWidth();
}

uint32_t CanvasImpl::Height() {
  return canvas_target_.GetHeight();
}

scoped_refptr<Rect> CanvasImpl::GetRect() {
  return Rect::New(0, 0, Width(), Height());
}

void CanvasImpl::Blt(int32_t x,
                     int32_t y,
                     scoped_refptr<Bitmap> src_bitmap,
                     scoped_refptr<Rect> src_rect,
                     uint32_t opacity) {
  if (x > Width() || y > Height())
    return;

  if (x + src_rect->Get_Width() < 0 || y + src_rect->Get_Height() < 0)
    return;

  StretchBlt(Rect::New(x, y, src_rect->Get_Width(), src_rect->Get_Height()),
             src_bitmap, src_rect, opacity);
}

void CanvasImpl::StretchBlt(scoped_refptr<Rect> dest_rect,
                            scoped_refptr<Bitmap> src_bitmap,
                            scoped_refptr<Rect> src_rect,
                            uint32_t opacity) {
  if (dest_rect->Get_X() > Width() || dest_rect->Get_Y() > Height())
    return;

  if (dest_rect->Get_X() + dest_rect->Get_Width() < 0 ||
      dest_rect->Get_Y() + dest_rect->Get_Height() < 0)
    return;

  // Synchronize draw commands
  auto* encoder = scheduler_->GetDrawContext()->GetImmediateEncoder();

  // Submit current texturt and src texture pending commands.
  SubmitQueuedCommands(*encoder);
  static_cast<CanvasImpl*>(src_bitmap.get())->SubmitQueuedCommands(*encoder);

  // Execute blt command
}

void CanvasImpl::FillRect(int32_t x,
                          int32_t y,
                          uint32_t width,
                          uint32_t height,
                          scoped_refptr<Color> color) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kFillRect;
  command->fill_rect.region = base::Rect(x, y, width, height);
  std::tie(std::ignore, command->fill_rect.color) =
      static_cast<ColorImpl*>(color.get())->FetchUpdateRequiredAndData();
}

void CanvasImpl::FillRect(scoped_refptr<Rect> rect,
                          scoped_refptr<Color> color) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kFillRect;
  command->fill_rect.region = static_cast<RectImpl*>(rect.get())->AsBaseRect();
  std::tie(std::ignore, command->fill_rect.color) =
      static_cast<ColorImpl*>(color.get())->FetchUpdateRequiredAndData();
}

void CanvasImpl::GradientFillRect(int32_t x,
                                  int32_t y,
                                  uint32_t width,
                                  uint32_t height,
                                  scoped_refptr<Color> color1,
                                  scoped_refptr<Color> color2,
                                  bool vertical) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kGradientFillRect;
  command->gradient_fill_rect.region = base::Rect(x, y, width, height);
  std::tie(std::ignore, command->gradient_fill_rect.color1) =
      static_cast<ColorImpl*>(color1.get())->FetchUpdateRequiredAndData();
  std::tie(std::ignore, command->gradient_fill_rect.color2) =
      static_cast<ColorImpl*>(color2.get())->FetchUpdateRequiredAndData();
  command->gradient_fill_rect.vertical = vertical;
}

void CanvasImpl::GradientFillRect(scoped_refptr<Rect> rect,
                                  scoped_refptr<Color> color1,
                                  scoped_refptr<Color> color2,
                                  bool vertical) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kGradientFillRect;
  command->gradient_fill_rect.region =
      static_cast<RectImpl*>(rect.get())->AsBaseRect();
  std::tie(std::ignore, command->gradient_fill_rect.color1) =
      static_cast<ColorImpl*>(color1.get())->FetchUpdateRequiredAndData();
  std::tie(std::ignore, command->gradient_fill_rect.color2) =
      static_cast<ColorImpl*>(color2.get())->FetchUpdateRequiredAndData();
  command->gradient_fill_rect.vertical = vertical;
}

void CanvasImpl::Clear() {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kFillRect;
  command->fill_rect.region = base::Rect(0, 0, Width(), Height());
  command->fill_rect.color = base::Vec4(0);
}

void CanvasImpl::ClearRect(int32_t x,
                           int32_t y,
                           uint32_t width,
                           uint32_t height) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kFillRect;
  command->fill_rect.region = base::Rect(x, y, width, height);
  command->fill_rect.color = base::Vec4(0);
}

void CanvasImpl::ClearRect(scoped_refptr<Rect> rect) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kFillRect;
  command->fill_rect.region = base::Rect(rect->Get_X(), rect->Get_Y(),
                                         rect->Get_Width(), rect->Get_Height());
  command->fill_rect.color = base::Vec4(0);
}

scoped_refptr<Color> CanvasImpl::GetPixel(int32_t x, int32_t y) {
  auto* cache_surface = FetchSurfacePixels();
  auto* pixels = static_cast<uint32_t*>(cache_surface->pixels);
  uint32_t single_pixel = *(pixels + x + y * cache_surface->w);

  uint8_t r, g, b, a;
  SDL_GetRGBA(single_pixel, SDL_GetPixelFormatDetails(cache_surface->format),
              nullptr, &r, &g, &b, &a);

  return new ColorImpl(base::Vec4(r, g, b, a));
}

void CanvasImpl::SetPixel(int32_t x, int32_t y, scoped_refptr<Color> color) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kFillRect;
  command->fill_rect.region = base::Rect(x, y, 1, 1);
  std::tie(std::ignore, command->fill_rect.color) =
      static_cast<ColorImpl*>(color.get())->FetchUpdateRequiredAndData();
}

void CanvasImpl::HueChange(int32_t hue) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kHueChange;
  command->hue_change.hue = hue;
}

void CanvasImpl::Blur() {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kBlur;
}

void CanvasImpl::RadialBlur(int32_t angle, int32_t division) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kRadialBlur;
  command->radial_blur.angle = angle;
  command->radial_blur.division = division;
}

void CanvasImpl::DrawText(int32_t x,
                          int32_t y,
                          uint32_t width,
                          uint32_t height,
                          const std::string& str,
                          int32_t align) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kDrawText;
  command->draw_text.text = str;
  command->draw_text.region = base::Rect(x, y, width, height);
  command->draw_text.text_align = align;
}

void CanvasImpl::DrawText(scoped_refptr<Rect> rect,
                          const std::string& str,
                          int32_t align) {
  CanvasDrawCommand* command = AllocateCommand();
  command->command_id = DrawCommandID::kDrawText;
  command->draw_text.text = str;
  command->draw_text.region = static_cast<RectImpl*>(rect.get())->AsBaseRect();
  command->draw_text.text_align = align;
}

scoped_refptr<Rect> CanvasImpl::TextSize(const std::string& str) {

  TTF_MeasureUTF8(,);

  return scoped_refptr<Rect>();
}

CanvasImpl::CanvasDrawCommand* CanvasImpl::AllocateCommand() {
  return nullptr;
}

scoped_refptr<Font> CanvasImpl::Get_Font() {
  return font_;
}

void CanvasImpl::Put_Font(const scoped_refptr<Font>& value) {
  font_ = value;
}

}  // namespace content
