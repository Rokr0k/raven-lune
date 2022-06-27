#pragma once

#include <string>
#include <SDL.h>

namespace rl
{
    namespace bga
    {
        void load(int key, const std::string &file);
        void start(int key);
        SDL_Surface *get(int key);
        void retrn(int key);
        void release();
    }
}