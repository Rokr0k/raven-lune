#include <rl/font.hpp>
#include <rl/utils.hpp>

namespace rl {
Font::Font(const std::string &filename) : m_Face{nullptr} {
  FT_New_Face(s_Library, filename.c_str(), 0, &m_Face);

  FT_Set_Pixel_Sizes(m_Face, 50, 50);
}

Font::~Font() { FT_Done_Face(m_Face); }

SDL_Surface *Font::RenderText(const std::string &text, int size) {
  FT_Set_Pixel_Sizes(m_Face, size, size);

  std::u32string u32text = utils::ConvertStringToU32(text);

  FT_GlyphSlot slot = m_Face->glyph;

  FT_Vector pen = {0, 0};
  FT_Pos xMin = 0x7fffffff;
  FT_Pos xMax = -0x7fffffff;
  FT_Pos yMin = 0x7fffffff;
  FT_Pos yMax = -0x7fffffff;

  for (const char32_t &c : u32text) {
    FT_Load_Char(m_Face, c, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);

    xMin = std::min(xMin, (pen.x >> 6) + slot->bitmap_left);
    xMax = std::max<FT_Pos>(xMax, (pen.x >> 6) + slot->bitmap_left +
                                      slot->bitmap.width);
    yMin = std::min(yMin, (pen.y >> 6) - slot->bitmap_top);
    yMax = std::max<FT_Pos>(yMax, (pen.y >> 6) - slot->bitmap_top +
                                      slot->bitmap.rows);

    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
      0, xMax - xMin, yMax - yMin, 32, SDL_PIXELFORMAT_RGBA32);

  SDL_Color colors[256];
  for (int i = 0; i < 256; i++) {
    colors[i].r = colors[i].g = colors[i].b = 0xff;
    colors[i].a = i;
  }

  pen = {-(xMin << 6), -(yMin << 6)};

  for (const char32_t &c : u32text) {
    FT_Load_Char(m_Face, c, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);

    SDL_Surface *glyph = SDL_CreateRGBSurfaceWithFormatFrom(
        slot->bitmap.buffer, slot->bitmap.width, slot->bitmap.rows, 8,
        slot->bitmap.pitch, SDL_PIXELFORMAT_INDEX8);
    SDL_SetPaletteColors(glyph->format->palette, colors, 0, 256);
    SDL_Rect dst = {(pen.x >> 6) + slot->bitmap_left,
                    (pen.y >> 6) - slot->bitmap_top,
                    static_cast<int>(slot->bitmap.width),
                    static_cast<int>(slot->bitmap.rows)};
    SDL_BlitSurface(glyph, nullptr, surface, &dst);
    SDL_FreeSurface(glyph);

    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  return surface;
}

SDL_Texture *Font::RenderText(SDL_Renderer *renderer, const std::string &text,
                              int size) {
  SDL_Surface *surface = RenderText(text, size);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void Font::Init() { FT_Init_FreeType(&s_Library); }

void Font::Quit() { FT_Done_FreeType(s_Library); }

FT_Library Font::s_Library = nullptr;
} // namespace rl