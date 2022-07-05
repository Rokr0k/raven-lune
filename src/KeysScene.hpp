#pragma once

#include "Scene.hpp"
#include <vector>

namespace rl
{
    class KeysScene : public Scene
    {
    private:
        int keyType;
        int mode;
        bool selected;
        bool pressed[27];
        SDL_FRect keyRects[27];
        SDL_Color keyColors[27];
        std::vector<std::pair<int, int>> ranges;

    public:
        KeysScene(App *app) : Scene(app) {}
        void initialise();
        void draw();
        void release();
        void onkeydown(SDL_KeyboardEvent key);
        void onkeyup(SDL_KeyboardEvent key);
    };
}