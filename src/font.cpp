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
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, text.c_str(), {0xff, 0xff, 0xff, 0xff});
    return surface;
}

SDL_Texture *font::renderText(SDL_Renderer *renderer, const std::string &text)
{
    SDL_Surface *temp = renderText(text);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    return texture;
}

void font::release()
{
    TTF_CloseFont(font_);
    TTF_Quit();
}