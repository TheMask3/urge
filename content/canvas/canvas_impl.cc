// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/canvas/canvas_impl.h"

#include "SDL3_ttf/SDL_ttf.h"

#include "content/canvas/canvas_scheduler.h"
#include "content/canvas/font_impl.h"
#include "content/common/color_impl.h"
#include "content/common/rect_impl.h"

namespace content {

scoped_refptr<Bitmap> Bitmap::New(ExecutionContext* execution_context,
                                  const std::string& filename,
                                  ExceptionState& exception_state) {
  return scoped_refptr<Bitmap>();
}

scoped_refptr<Bitmap> Bitmap::New(ExecutionContext* execution_context,
                                  uint32_t width,
                                  uint32_t height,
                                  ExceptionState& exception_state) {
  return scoped_refptr<Bitmap>();
}

scoped_refptr<Bitmap> Bitmap::Copy(scoped_refptr<Bitmap> other) {
  return scoped_refptr<Bitmap>();
}

scoped_refptr<Bitmap> Bitmap::Deserialize(const std::string&,
                                          ExceptionState& exception_state) {}

std::string Bitmap::Serialize(scoped_refptr<Bitmap>,
                              ExceptionState& exception_state) {}

CanvasImpl::CanvasImpl(wgpu::Texture canvas_texture, CanvasScheduler* scheduler)
    : canvas_texture_(canvas_texture), scheduler_(scheduler) {}

CanvasImpl::~CanvasImpl() {
  ExceptionState exception_state;
  Dispose(exception_state);
}

SDL_Surface* CanvasImpl::RequireMemorySurface() {
  // Submit pending commands
  auto* encoder = scheduler_->GetDrawContext()->GetImmediateEncoder();
  SubmitQueuedCommands(*encoder);

  // Synchronize immediate context and flush
  scheduler_->GetDrawContext()->Flush();

  // Fetch buffer to texture

  return nullptr;
}

void CanvasImpl::SubmitQueuedCommands(const wgpu::CommandEncoder& encoder) {
  Command* command_sequence = nullptr;
  while (command_sequence) {
    switch (command_sequence->id) {
      case CommandID::kFillRect:
        break;
      case CommandID::kGradientFillRect:
        break;
      case CommandID::kHueChange:
        break;
      case CommandID::kBlur:
        break;
      case CommandID::kRadialBlur:
        break;
      case CommandID::kDrawText:
        break;
      default:
        break;
    }

    command_sequence = command_sequence->next;
  }
}

void CanvasImpl::Dispose(ExceptionState& exception_state) {
  if (!IsDisposed(exception_state)) {
    canvas_texture_.Destroy();
    canvas_texture_ = nullptr;
  }
}

bool CanvasImpl::IsDisposed(ExceptionState& exception_state) {
  return !canvas_texture_.Get();
}

uint32_t CanvasImpl::Width(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;
  return canvas_texture_.GetWidth();
}

uint32_t CanvasImpl::Height(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return 0;
  return canvas_texture_.GetHeight();
}

scoped_refptr<Rect> CanvasImpl::GetRect(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;
  return Rect::New(0, 0, canvas_texture_.GetWidth(),
                   canvas_texture_.GetHeight(), exception_state);
}

void CanvasImpl::Blt(int32_t x,
                     int32_t y,
                     scoped_refptr<Bitmap> src_bitmap,
                     scoped_refptr<Rect> src_rect,
                     uint32_t opacity,
                     ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CanvasImpl* src_canvas = static_cast<CanvasImpl*>(src_bitmap.get());
  BlitTextureInternal(base::Rect(x, y, src_rect->Get_Width(exception_state),
                                 src_rect->Get_Height(exception_state)),
                      src_canvas->canvas_texture_,
                      RectImpl::From(src_rect)->AsBaseRect());
}

void CanvasImpl::StretchBlt(scoped_refptr<Rect> dest_rect,
                            scoped_refptr<Bitmap> src_bitmap,
                            scoped_refptr<Rect> src_rect,
                            uint32_t opacity,
                            ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  CanvasImpl* src_canvas = static_cast<CanvasImpl*>(src_bitmap.get());
  BlitTextureInternal(RectImpl::From(dest_rect)->AsBaseRect(),
                      src_canvas->canvas_texture_,
                      RectImpl::From(src_rect)->AsBaseRect());
}

void CanvasImpl::FillRect(int32_t x,
                          int32_t y,
                          uint32_t width,
                          uint32_t height,
                          scoped_refptr<Color> color,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_FillRect>();
  command->region = base::Rect(x, y, width, height);
  command->color = ColorImpl::From(color.get())->AsNormColor();
}

void CanvasImpl::FillRect(scoped_refptr<Rect> rect,
                          scoped_refptr<Color> color,
                          ExceptionState& exception_state) {
  FillRect(rect->Get_X(exception_state), rect->Get_Y(exception_state),
           rect->Get_Width(exception_state), rect->Get_Height(exception_state),
           color, exception_state);
}

void CanvasImpl::GradientFillRect(int32_t x,
                                  int32_t y,
                                  uint32_t width,
                                  uint32_t height,
                                  scoped_refptr<Color> color1,
                                  scoped_refptr<Color> color2,
                                  bool vertical,
                                  ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_GradientFillRect>();
  command->region = base::Rect(x, y, width, height);
  command->color1 = ColorImpl::From(color1.get())->AsNormColor();
  command->color2 = ColorImpl::From(color2.get())->AsNormColor();
  command->vertical = vertical;
}

void CanvasImpl::GradientFillRect(scoped_refptr<Rect> rect,
                                  scoped_refptr<Color> color1,
                                  scoped_refptr<Color> color2,
                                  bool vertical,
                                  ExceptionState& exception_state) {
  GradientFillRect(rect->Get_X(exception_state), rect->Get_Y(exception_state),
                   rect->Get_Width(exception_state),
                   rect->Get_Height(exception_state), color1, color2, vertical,
                   exception_state);
}

void CanvasImpl::Clear(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_FillRect>();
  command->region =
      base::Rect(0, 0, Width(exception_state), Height(exception_state));
  command->color = base::Vec4(0);
}

void CanvasImpl::ClearRect(int32_t x,
                           int32_t y,
                           uint32_t width,
                           uint32_t height,
                           ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_FillRect>();
  command->region =
      base::Rect(0, 0, Width(exception_state), Height(exception_state));
  command->color = base::Vec4(0);
}

void CanvasImpl::ClearRect(scoped_refptr<Rect> rect,
                           ExceptionState& exception_state) {
  ClearRect(rect->Get_X(exception_state), rect->Get_Y(exception_state),
            rect->Get_Width(exception_state), rect->Get_Height(exception_state),
            exception_state);
}

scoped_refptr<Color> CanvasImpl::GetPixel(int32_t x,
                                          int32_t y,
                                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  SDL_Surface* pixels = RequireMemorySurface();
}

void CanvasImpl::SetPixel(int32_t x,
                          int32_t y,
                          scoped_refptr<Color> color,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_FillRect>();
  command->region = base::Rect(x, y, 1, 1);
  command->color = ColorImpl::From(color.get())->AsNormColor();
}

void CanvasImpl::HueChange(int32_t hue, ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_HueChange>();
  command->hue = hue;
}

void CanvasImpl::Blur(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_RadialBlur>();
  command->blur = true;
}

void CanvasImpl::RadialBlur(int32_t angle,
                            int32_t division,
                            ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* command = AllocateCommand<Command_RadialBlur>();
  command->blur = false;
  command->angle = angle;
  command->division = division;
}

void CanvasImpl::DrawText(int32_t x,
                          int32_t y,
                          uint32_t width,
                          uint32_t height,
                          const std::string& str,
                          int32_t align,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;

  auto* font_object = static_cast<FontImpl*>(font_.get());
  if (!font_object->GetCanonicalFont(exception_state))
    return;

  auto* text_surface = font_object->RenderText(str, 0, exception_state);
  if (!text_surface)
    return;

  // Destroy memory surface in queue deferrer.
  auto* command = AllocateCommand<Command_DrawText>();
  command->region = base::Rect(x, y, width, height);
  command->text = text_surface;
  command->align = align;
}

void CanvasImpl::DrawText(scoped_refptr<Rect> rect,
                          const std::string& str,
                          int32_t align,
                          ExceptionState& exception_state) {
  DrawText(rect->Get_X(exception_state), rect->Get_Y(exception_state),
           rect->Get_Width(exception_state), rect->Get_Height(exception_state),
           str, align, exception_state);
}

scoped_refptr<Rect> CanvasImpl::TextSize(const std::string& str,
                                         ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;

  TTF_Font* font =
      static_cast<FontImpl*>(font_.get())->GetCanonicalFont(exception_state);
  if (!font)
    return nullptr;

  int w, h;
  TTF_GetStringSize(font, str.c_str(), str.size(), &w, &h);
  return Rect::New(0, 0, w, h, exception_state);
}

bool CanvasImpl::CheckDisposed(ExceptionState& exception_state) {
  if (IsDisposed(exception_state)) {
    exception_state.ThrowContentError(ExceptionCode::kDisposedObject,
                                      "disposed object: bitmap");
    return true;
  }

  return false;
}

void CanvasImpl::BlitTextureInternal(const base::Rect& dst_rect,
                                     const wgpu::Texture& src_texture,
                                     const base::Rect& src_rect) {}

scoped_refptr<Font> CanvasImpl::Get_Font(ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return nullptr;
  return font_;
}

void CanvasImpl::Put_Font(const scoped_refptr<Font>& value,
                          ExceptionState& exception_state) {
  if (CheckDisposed(exception_state))
    return;
  font_ = value;
}

}  // namespace content
