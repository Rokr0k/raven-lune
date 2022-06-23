#pragma once

#include "Scene.hpp"
#include <bmsparser/bmsparser.hpp>
#include <list>
#include <chrono>

namespace rl
{
    class PlayScene : public Scene
    {
    private:
        bms::Chart *chart;
        bool *executed;
        enum class PlayMode
        {
            SINGLE,
            DUAL
        } playMode;
        std::chrono::system_clock::time_point timer;
        float speed;
        SDL_Texture *bpmDisplay;
        SDL_FRect bpmDisplayRect;
        size_t sectorIdx;

    public:
        PlayScene(App *app, bms::Chart *chart) : Scene(app), chart(chart) {}
        void initialise(SDL_Renderer *renderer);
        void draw(SDL_Renderer *renderer);
        void release();
        void onkeydown(SDL_KeyboardEvent key);
        void onkeyup(SDL_KeyboardEvent key);
    };
}