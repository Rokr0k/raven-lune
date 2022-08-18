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
        Scene() {}
        virtual void initialise(App *app)
        {
            this->app = app;
        }
        virtual void draw() {}
        virtual void release() {}
        virtual void onkeydown(SDL_KeyboardEvent key) {}
        virtual void onkeyup(SDL_KeyboardEvent key) {}
    };
}