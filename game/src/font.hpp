#pragma once

#include <SDL_surface.h>
#include <SDL_render.h>
#include <ft2build.h>
#include <string>

#include FT_FREETYPE_H

namespace rl {
class Font {
public:
  Font(const std::string &filename);
  ~Font();

  SDL_Surface *RenderText(const std::string &text);
  SDL_Texture *RenderText(SDL_Renderer *renderer, const std::string &text);

  static void Init();
  static void Quit();

private:
  static FT_Library s_Library;

  FT_Face m_Face;
};
} // namespace rl