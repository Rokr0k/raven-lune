#pragma once

#include "Scene.hpp"

namespace rl
{
    class ResultScene : public Scene
    {
    public:
        struct Result
        {
            bool cleared;
            int judgeCount[5];
            int maxCombo;
            size_t noteCount;
        };

    private:
        Result result;

        SDL_Texture *cleared;
        SDL_FRect clearedRect;
        SDL_Texture *maxCombo;
        SDL_FRect maxComboRect;
        SDL_Texture *judge[5];
        SDL_FRect judgeRect[5];
        SDL_Texture *rank;
        SDL_FRect rankRect;

    public:
        ResultScene(Result result) : result(result) {}
        void initialise(App *app);
        void draw();
        void release();
        void onkeydown(SDL_KeyboardEvent key);
    };
}