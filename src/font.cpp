#include "font.hpp"
#include <SDL_ttf.h>

using namespace rl;

static TTF_Font *font_;

void font::initialise(const std::string &file)
{
    TTF_Init();
    font_ = TTF_OpenFont(file.c_str(), 50);
}

SDL_Surface *font::renderText(const std::string &text)
{
    SDL_Color fg{0xff, 0xff, 0xff, 0xff};
    SDL_Color bg{0x00, 0x00, 0x00, 0x00};
    return TTF_RenderText(font_, text.c_str(), fg, bg);
}

void font::release()
{
    TTF_CloseFont(font_);
    TTF_Quit();
}