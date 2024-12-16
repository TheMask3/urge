// Copyright 2024 Admenri.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CANVAS_FONT_IMPL_H_
#define CONTENT_CANVAS_FONT_IMPL_H_

#include <map>
#include <string>

#include "SDL3/SDL_surface.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "components/filesystem/io.h"
#include "content/common/color_impl.h"
#include "content/content_config.h"
#include "content/public/engine_font.h"

namespace content {

class ScopedFontData {
 public:
  ScopedFontData(const std::string& default_font_name, filesystem::IO* io);
  ~ScopedFontData();

  ScopedFontData(const ScopedFontData&) = delete;
  ScopedFontData& operator=(const ScopedFontData&) = delete;

  void* GetUIDefaultFont(int64_t* font_size);

  bool Existed(const std::string& name);
  void SetDefaultName(const std::vector<std::string>& name);
  std::vector<std::string> GetDefaultName();
  void SetDefaultSize(int size);
  int GetDefaultSize();
  void SetDefaultBold(bool bold);
  bool GetDefaultBold();
  void SetDefaultItalic(bool italic);
  bool GetDefaultItalic();
  void SetDefaultShadow(bool shadow);
  bool GetDefaultShadow();
  void SetDefaultOutline(bool outline);
  bool GetDefaultOutline();
  void SetDefaultColor(scoped_refptr<Color> color);
  scoped_refptr<Color> GetDefaultColor();
  void SetDefaultOutColor(scoped_refptr<Color> color);
  scoped_refptr<Color> GetDefaultOutColor();

 private:
  friend class FontImpl;

  std::vector<std::string> default_name_;
  int default_size_ = 24;
  bool default_bold_ = false;
  bool default_italic_ = false;
  bool default_outline_ = true;
  bool default_shadow_ = false;
  scoped_refptr<Color> default_color_ = nullptr;
  scoped_refptr<Color> default_out_color_ = nullptr;

  std::string font_default_name_;
  std::map<std::pair<std::string, int>, TTF_Font*> font_cache_;
  std::map<std::string, std::pair<int64_t, void*>> cache_data_;
};

class FontImpl : public Font {
 public:
  FontImpl();
  ~FontImpl() override;

  FontImpl(const FontImpl&) = delete;
  FontImpl& operator=(const FontImpl&) = delete;

  void EnsureLoadFont();
  TTF_Font* AsSDLFont();
  SDL_Surface* RenderText(const std::string& text, uint8_t* font_opacity);

 protected:
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Name, std::vector<std::string>);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Size, uint32_t);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Bold, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Italic, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Outline, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Shadow, bool);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(Color, scoped_refptr<Color>);
  URGE_DECLARE_OVERRIDE_ATTRIBUTE(OutColor, scoped_refptr<Color>);

 private:
  void LoadFontInternal();

  std::vector<std::string> name_;
  int size_;
  bool bold_;
  bool italic_;
  bool outline_;
  bool shadow_;
  scoped_refptr<Color> color_;
  scoped_refptr<Color> out_color_;

  ScopedFontData* parent_;
  TTF_Font* font_;
};

}  // namespace content

#endif  //! CONTENT_CANVAS_FONT_IMPL_H_
