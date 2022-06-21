#pragma once

#include <SDL.h>
#include "App.hpp"

namespace rl
{
    class App;

    class Scene
    {
    protected:
        App *app;

    public:
        Scene(App *app) : app(app) {}
        virtual void initialise(SDL_Renderer *renderer) {}
        virtual void draw(SDL_Renderer *renderer) {}
        virtual void release() {}
        virtual void onkeydown(SDL_KeyboardEvent key) {}
        virtual void onkeyup(SDL_KeyboardEvent key) {}
    };
}