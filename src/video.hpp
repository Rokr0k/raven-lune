#pragma once

#include <string>
#include <SDL.h>

namespace rl
{
    namespace video
    {
        void initialise();
        void load(int key, const std::string &file);
        void play(int key);
        SDL_Surface *get(int key);
        void retrn(int key);
        void release();
    }
}