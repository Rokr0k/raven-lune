#include "font.hpp"
#include "utils.hpp"

namespace rl {
Font::Font(const std::string &filename) : m_Face{nullptr} {
  FT_New_Face(s_Library, filename.c_str(), 0, &m_Face);

  FT_Set_Char_Size(m_Face, 100 * 64, 0, 300, 0);
}

Font::~Font() { FT_Done_Face(m_Face); }

SDL_Surface *Font::RenderText(const std::string &text) {
  std::u32string u32text = utils::ConvertStringToU32(text);

  FT_Vector pen{0, 0};
  FT_Pos xMin = 0x7fffffff;
  FT_Pos xMax = -0x7fffffff;
  FT_Pos yMin = 0x7fffffff;
  FT_Pos yMax = -0x7fffffff;

  for (const char32_t &c : u32text) {
    int charIdx = FT_Get_Char_Index(m_Face, c);
    FT_Load_Char(m_Face, charIdx, FT_LOAD_DEFAULT);

    xMin = std::min(xMin, pen.x + m_Face->glyph->metrics.horiBearingX);
    xMax = std::max(xMax, pen.x + m_Face->glyph->metrics.horiBearingX +
                              m_Face->glyph->metrics.width);
    yMin = std::min(yMin, pen.y + m_Face->glyph->metrics.horiBearingY);
    yMax = std::max(yMax, pen.y + m_Face->glyph->metrics.horiBearingY -
                              m_Face->glyph->metrics.height);

    pen.x += m_Face->glyph->metrics.horiAdvance;
  }

  return nullptr;
}

SDL_Texture *Font::RenderText(SDL_Renderer *renderer, const std::string &text) {
  return SDL_CreateTextureFromSurface(renderer, RenderText(text));
}

void Font::Init() { FT_Init_FreeType(&s_Library); }

void Font::Quit() { FT_Done_FreeType(s_Library); }

FT_Library Font::s_Library = nullptr;
} // namespace rl