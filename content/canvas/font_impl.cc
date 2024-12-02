// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/canvas/font_impl.h"

#include <map>

#include "content/common/color_impl.h"

namespace content {

const int kOutlineSize = 1;

namespace {

void RenderShadowSurface(SDL_Surface*& in, const SDL_Color& color) {
  // TODO:
}

std::pair<int64_t, void*> ReadFontToMemory(SDL_IOStream* io) {
  int64_t fsize = SDL_GetIOSize(io);
  void* mem = SDL_malloc(fsize);
  if (mem) {
    SDL_ReadIO(io, mem, fsize);
    SDL_CloseIO(io);
  }

  return std::make_pair(fsize, mem);
}

TTF_Font* ReadFontFromMemory(
    const std::map<std::string, std::pair<int64_t, void*>>& mem_fonts,
    const std::string& path,
    int size) {
  auto it = mem_fonts.find(path);
  if (it != mem_fonts.end()) {
    SDL_IOStream* io = SDL_IOFromConstMem(it->second.second, it->second.first);
    return TTF_OpenFontIO(io, true, size * 0.9f);
  }

  return nullptr;
}

}  // namespace

ScopedFontData::ScopedFontData(const std::string& default_font_name,
                               filesystem::Filesystem* io)
    : default_color_(Color::New(255.0, 255.0, 255.0, 255.0)),
      default_out_color_(Color::New(0, 0, 0, 255.0)) {
  std::string filename(default_font_name);
  std::string dir("."), file;

  size_t last_slash_pos = filename.find_last_of('/');
  if (last_slash_pos != std::string::npos) {
    dir = filename.substr(0, last_slash_pos);
    file = filename.substr(last_slash_pos + 1);
  } else
    file = filename;

  dir.push_back('/');
  font_default_name_ = file;
  LOG(INFO) << "[Font] Search Path: " << dir;
  LOG(INFO) << "[Font] Default Font: " << file;
  default_name_.push_back(file);

  auto font_files = io->EnumDir(dir);
  for (auto& file_iter : font_files) {
    std::string filepath = dir + file_iter;

    SDL_IOStream* font_ops = nullptr;
    try {
      font_ops = io->OpenReadRaw(filepath);
    } catch (...) {
      // Ignore font load error
      font_ops = nullptr;
    }

    // Cached in memory
    if (font_ops) {
      LOG(INFO) << "[Font] Loaded Font: " << file_iter;
      mem_fonts_.emplace(file_iter, ReadFontToMemory(font_ops));
    }
  }
}

ScopedFontData::~ScopedFontData() {
  for (auto& it : font_cache_)
    TTF_CloseFont(it.second);

  for (auto& it : mem_fonts_)
    SDL_free(it.second.second);
}

void* ScopedFontData::GetUIDefaultFont(int64_t* font_size) {
  auto it = mem_fonts_.find(font_default_name_);
  if (it != mem_fonts_.end()) {
    *font_size = it->second.first;
    return it->second.second;
  }

  return nullptr;
}

bool ScopedFontData::Existed(const std::string& name) {
  auto it = mem_fonts_.find(name);
  if (it != mem_fonts_.end())
    return true;

  return false;
}

void ScopedFontData::SetDefaultName(const std::vector<std::string>& name) {
  default_name_ = name;
}

std::vector<std::string> ScopedFontData::GetDefaultName() {
  return default_name_;
}

void ScopedFontData::SetDefaultSize(int size) {
  default_size_ = size;
}

int ScopedFontData::GetDefaultSize() {
  return default_size_;
}

void ScopedFontData::SetDefaultBold(bool bold) {
  default_bold_ = bold;
}

bool ScopedFontData::GetDefaultBold() {
  return default_bold_;
}

void ScopedFontData::SetDefaultItalic(bool italic) {
  default_italic_ = italic;
}

bool ScopedFontData::GetDefaultItalic() {
  return default_italic_;
}

void ScopedFontData::SetDefaultShadow(bool shadow) {
  default_shadow_ = shadow;
}

bool ScopedFontData::GetDefaultShadow() {
  return default_shadow_;
}

void ScopedFontData::SetDefaultOutline(bool outline) {
  default_outline_ = outline;
}

bool ScopedFontData::GetDefaultOutline() {
  return default_outline_;
}

void ScopedFontData::SetDefaultColor(scoped_refptr<Color> color) {
  default_color_->Set(color);
}

scoped_refptr<Color> ScopedFontData::GetDefaultColor() {
  return default_color_;
}

void ScopedFontData::SetDefaultOutColor(scoped_refptr<Color> color) {
  default_out_color_->Set(color);
}

scoped_refptr<Color> ScopedFontData::GetDefaultOutColor() {
  return default_out_color_;
}

std::vector<std::string> FontImpl::Get_Name() {
  return name_;
}

void FontImpl::Put_Name(const std::vector<std::string>& value) {
  name_ = value;
}

uint32_t FontImpl::Get_Size() {
  return size_;
}

void FontImpl::Put_Size(const uint32_t& value) {
  size_ = value;
}

bool FontImpl::Get_Italic() {
  return italic_;
}

void FontImpl::Put_Italic(const bool& value) {
  italic_ = value;
}

bool FontImpl::Get_Outline() {
  return outline_;
}

void FontImpl::Put_Outline(const bool& value) {
  outline_ = value;
}

bool FontImpl::Get_Shadow() {
  return shadow_;
}

void FontImpl::Put_Shadow(const bool& value) {
  shadow_ = value;
}

scoped_refptr<Color> FontImpl::Get_Color() {
  return color_;
}

void FontImpl::Put_Color(const scoped_refptr<Color>& value) {
  color_ = value;
}

scoped_refptr<Color> FontImpl::Get_OutColor() {
  return out_color_;
}

void FontImpl::Put_OutColor(const scoped_refptr<Color>& value) {
  out_color_ = value;
}

void FontImpl::EnsureLoadFont() {
  if (!font_)
    LoadFontInternal();
}

TTF_Font* FontImpl::AsSDLFont() {
  EnsureLoadFont();

  int font_style = TTF_STYLE_NORMAL;
  if (bold_)
    font_style |= TTF_STYLE_BOLD;
  if (italic_)
    font_style |= TTF_STYLE_ITALIC;

  TTF_SetFontStyle(font_, font_style);

  return font_;
}

SDL_Surface* FontImpl::RenderText(const std::string& text,
                                  uint8_t* font_opacity) {
  auto ensure_format = [](SDL_Surface*& surf) {
    SDL_Surface* format_surf = nullptr;
    if (surf->format != SDL_PIXELFORMAT_ABGR8888) {
      format_surf = SDL_ConvertSurface(surf, SDL_PIXELFORMAT_ABGR8888);
      SDL_DestroySurface(surf);
      surf = format_surf;
    }
  };

  std::string src_text(text);
  if (src_text.empty() || src_text == " ")
    return nullptr;

  TTF_Font* font = AsSDLFont();
  if (!font)
    return nullptr;

  ColorImpl* color_impl = static_cast<ColorImpl*>(color_.get());
  ColorImpl* out_color_impl = static_cast<ColorImpl*>(out_color_.get());

  SDL_Color font_color = color_impl->AsSDLColor();
  SDL_Color outline_color = out_color_impl->AsSDLColor();
  if (font_opacity)
    *font_opacity = font_color.a;

  font_color.a = 255;
  outline_color.a = 255;

  SDL_Surface* raw_surf =
      TTF_RenderUTF8_Blended(font, src_text.c_str(), font_color);
  if (!raw_surf)
    return nullptr;
  ensure_format(raw_surf);

  if (shadow_)
    RenderShadowSurface(raw_surf, font_color);

  if (outline_) {
    SDL_Surface* outline = nullptr;
    TTF_SetFontOutline(font, kOutlineSize);
    outline = TTF_RenderUTF8_Blended(font, src_text.c_str(), outline_color);
    ensure_format(outline);
    SDL_Rect outRect = {kOutlineSize, kOutlineSize, raw_surf->w, raw_surf->h};
    SDL_SetSurfaceBlendMode(raw_surf, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(raw_surf, NULL, outline, &outRect);
    SDL_DestroySurface(raw_surf);
    raw_surf = outline;
    TTF_SetFontOutline(font, 0);
  }

  return raw_surf;
}

void FontImpl::LoadFontInternal() {
  std::vector<std::string> load_names(name_);
  load_names.push_back(parent_->font_default_name_);

  auto& font_cache = parent_->font_cache_;
  if (size_ >= 6 && size_ <= 96) {
    for (const auto& font_name : load_names) {
      // Find existed font object
      auto it = font_cache.find(std::make_pair(font_name, size_));
      if (it != font_cache.end()) {
        font_ = it->second;
        return;
      }

      // Load new font object
      TTF_Font* font_obj =
          ReadFontFromMemory(parent_->mem_fonts_, font_name, size_);
      if (font_obj) {
        // Storage new font obj
        font_ = font_obj;
        font_cache.emplace(std::make_pair(font_name, size_), font_);
        return;
      }
    }
  }

  // Failed to load font
  std::string font_names;
  for (auto& it : name_)
    font_names = font_names + it + " ";
  throw base::Exception(base::Exception::ContentError,
                        "Failed to load Font: %s", font_names.c_str());
}

}  // namespace content
