#pragma once

#include <SDL.h>
#include "Scene.hpp"

namespace rl
{
    class Scene;
    
    class App
    {
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        Scene *scene;

    public:
        App(Scene *initialScene);
        ~App();
        int loop();
        void changeScene(Scene *newScene);
    };
}