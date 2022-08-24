#pragma once

#include "Scene.hpp"
#include <bmsparser.hpp>
#include <map>
#include <chrono>

namespace rl
{
    class PlayScene : public Scene
    {
    private:
        class Obj : public bms::Obj
        {
        public:
            bool executed;
            Obj(const bms::Obj &obj) : bms::Obj(obj), executed(false) {}
        };
        bms::Chart *chart;
        std::vector<Obj> objs;
        bool automatic;
        Uint32 timer;
        int speed;
        SDL_Texture *speedDisplay;
        SDL_FRect speedDisplayRect;
        SDL_Texture *bpmDisplay;
        SDL_FRect bpmDisplayRect;
        float judgeTime;
        enum class JudgeType
        {
            NONE,
            JUST,
            GREAT,
            GOOD,
            BAD,
            POOR,
            GPOOR,
        } judgeType;
        int combo;
        SDL_Texture *judgeDisplay;
        SDL_FRect judgeDisplayRect[2];
        int maxCombo;
        float gauge;
        SDL_FRect gaugeDisplayRect[2];
        int judgeCount[5];
        size_t sectorIdx;
        std::map<std::pair<int, int>, bool> pressed;

        std::map<std::pair<int, int>, JudgeType> suspendedJudge;

        std::vector<std::pair<float, std::pair<int, int>>> keyupList;

        int bgas[3];
        float bgaStarted[3];
        SDL_FRect bgaRect;

        size_t noteCnt;

        SDL_Texture *autoplayDisplay;
        SDL_FRect autoplayDisplayRect;

        void keydown(int player, int line);
        void keyup(int player, int line);

        void judge(JudgeType j);

    public:
        PlayScene(bms::Chart *chart, bool automatic = false) : chart(chart), automatic(automatic) {}
        void initialise(App *app);
        void draw();
        void release();
        void onkeydown(SDL_KeyboardEvent key);
        void onkeyup(SDL_KeyboardEvent key);
    };
}