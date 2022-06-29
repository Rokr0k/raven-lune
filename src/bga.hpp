#pragma once

#include <string>
#include <SDL.h>

namespace rl
{
    namespace bga
    {
        void load(SDL_Renderer *,int key, const std::string &file);
        SDL_Texture *get(int key);
        void release();
    }
}