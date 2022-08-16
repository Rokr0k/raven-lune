#pragma once

#include <string>
#include <SDL.h>

namespace rl
{
    namespace bga
    {
        void load(int key, const std::string &file);
        SDL_Surface *get(int key, float time);
        void release();
    }
}