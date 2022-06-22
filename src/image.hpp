#pragma once

#include <SDL.h>
#include <string>

namespace rl
{
    namespace image
    {
        void initialise();
        SDL_Surface *loadImage(const std::string &file);
        SDL_Texture *loadImage(SDL_Renderer *renderer, const std::string &file);
        void release();
    }
}