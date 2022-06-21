#pragma once

#include "Scene.hpp"
#include <future>

namespace rl
{
    class ListScene : public Scene
    {
    public:
        ListScene(App *app) : Scene(app) {}
        void initialise(SDL_Renderer *renderer);
        void draw(SDL_Renderer *renderer);
        void release();
    };
}