#include "PlayScene.hpp"
#include "ListScene.hpp"
#include "file.hpp"
#include "audio.hpp"

using namespace rl;

void PlayScene::initialise(SDL_Renderer *renderer)
{
    file::release();
    for (int i = 0; i < 1296; i++)
    {
        if (!chart->wavs[i].empty())
        {
            audio::loadAudio(i, chart->wavs[i]);
        }
    }
    executed = new bool[chart->objs.size()];
    for (size_t i = 0; i < chart->objs.size(); i++)
    {
        executed[i] = false;
    }

    timer = std::chrono::high_resolution_clock::now();
}

void PlayScene::draw(SDL_Renderer *renderer)
{
    bool empty = true;
    for (size_t i = 0; i < chart->objs.size(); i++)
    {
        if (executed[i])
        {
            continue;
        }
        empty = false;
        if (chart->objs[i].time > (std::chrono::high_resolution_clock::now() - timer).count() * 0.000000001f)
        {
            continue;
        }
        executed[i] = true;
        switch (chart->objs[i].type)
        {
        case bms::Obj::Type::BGM:
            audio::playAudio(chart->objs[i].bgm.key);
            break;
        case bms::Obj::Type::NOTE:
            if (!chart->objs[i].note.end)
            {
                audio::playAudio(chart->objs[i].note.key);
            }
            break;
        case bms::Obj::Type::BMP:
            break;
        case bms::Obj::Type::BOMB:
        case bms::Obj::Type::INVISIBLE:
            break;
        }
    }
    if (empty && !audio::isPlayingAudio())
    {
        app->changeScene(new ListScene(app));
    }
}

void PlayScene::release()
{
    delete chart;
    file::initialise();
}

void PlayScene::onkeydown(SDL_KeyboardEvent key)
{
    //
}

void PlayScene::onkeyup(SDL_KeyboardEvent key)
{
    //
}