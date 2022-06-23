#pragma once

#include <SDL.h>
#include "Scene.hpp"

namespace rl
{
    class Scene;

    class App
    {
    private:
        Scene *scene;

    public:
        SDL_Window *window;
        SDL_Renderer *renderer;

        App(Scene *initialScene);
        ~App();
        int loop();
        void changeScene(Scene *newScene);
    };
}