#pragma once

#include <string>
#include <SDL.h>

namespace rl
{
    namespace font
    {
        void initialise(const std::string &file);
        SDL_Surface *renderText(const std::string &text);
        void release();
    }
}