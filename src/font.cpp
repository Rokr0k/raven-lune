#include "font.hpp"
#include <map>
#include <SDL_ttf.h>

using namespace rl;

static std::string font_;
static std::map<int, TTF_Font *> fonts;

void font::initialise(const std::string &file)
{
    TTF_Init();
    font_ = file;
}

void font::loadSize(int size)
{
    if (fonts.find(size) == fonts.end())
    {
        fonts[size] = TTF_OpenFont(font_.c_str(), size);
    }
}

void font::unloadSize(int size)
{
    if (fonts.find(size) != fonts.end())
    {
        TTF_CloseFont(fonts[size]);
        fonts.erase(size);
    }
}

SDL_Surface *font::renderText(const std::string &text, int size)
{
    if (fonts.find(size) != fonts.end())
    {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(fonts[size], text.c_str(), {0xff, 0xff, 0xff, 0xff});
        return surface;
    }
    else
    {
        TTF_Font *f = TTF_OpenFont(font_.c_str(), size);
        SDL_Surface *surface = TTF_RenderUTF8_Blended(f, text.c_str(), {0xff, 0xff, 0xff, 0xff});
        TTF_CloseFont(f);
        return surface;
    }
}

SDL_Texture *font::renderText(SDL_Renderer *renderer, const std::string &text, int size)
{
    SDL_Surface *temp = renderText(text, size);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    return texture;
}

void font::release()
{
    for (std::pair<const int, TTF_Font *> &f : fonts)
    {
        TTF_CloseFont(f.second);
    }
    fonts.clear();

    TTF_Quit();
}