#pragma once

#include "Scene.hpp"
#include <bmsparser/bmsparser.hpp>
#include <map>
#include <chrono>

namespace rl
{
    class PlayScene : public Scene
    {
    private:
        bms::Chart *chart;
        enum class PlayMode
        {
            SINGLE,
            DUAL
        } playMode;
        bool automatic;
        Uint32 timer;
        float speed;
        SDL_Texture *bpmDisplay;
        SDL_FRect bpmDisplayRect;
        float judgeTime;
        SDL_Texture *judgeDisplay;
        SDL_FRect judgeDisplayRect[2];
        size_t sectorIdx;
        std::map<std::pair<int, int>, bool> pressed;

        size_t noteCnt;

        int combo;
        float gauge;

        void keydown(int player, int line);
        void keyup(int player, int line);

        enum class JudgeType
        {
            JUST,
            GREAT,
            GOOD,
            BAD,
            POOR,
            GPOOR
        };
        void judge(JudgeType j);

    public:
        PlayScene(App *app, bms::Chart *chart, bool automatic = false) : Scene(app), chart(chart), automatic(automatic) {}
        void initialise();
        void draw();
        void release();
        void onkeydown(SDL_KeyboardEvent key);
        void onkeyup(SDL_KeyboardEvent key);
    };
}