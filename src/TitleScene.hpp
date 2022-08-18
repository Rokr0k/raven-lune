#pragma once

#include "Scene.hpp"

namespace rl
{
    class TitleScene : public Scene
    {
    private:
        SDL_Texture *title;
        SDL_FRect titleRect;
        SDL_Texture *instruction;
        SDL_FRect instructionRect;

    public:
        void initialise(App *app);
        void draw();
        void release();
        void onkeydown(SDL_KeyboardEvent key);
    };
}