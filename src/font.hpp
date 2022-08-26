#pragma once

#include <string>
#include <SDL.h>

namespace rl
{
    namespace font
    {
        void initialise(const std::string &file);
        void loadSize(int size);
        void unloadSize(int size);
        SDL_Surface *renderText(const std::string &text, int size);
        SDL_Texture *renderText(SDL_Renderer *renderer, const std::string &text, int size);
        void release();
    }
}