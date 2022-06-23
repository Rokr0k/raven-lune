#include "PlayScene.hpp"
#include "ListScene.hpp"
#include "file.hpp"
#include "audio.hpp"
#include "image.hpp"

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
    notes = image::loadImage(renderer, "res/notes.png");
    bombs = image::loadImage(renderer, "res/bombs.png");

    timer = std::chrono::high_resolution_clock::now();

    speed = 2;
}

void PlayScene::draw(SDL_Renderer *renderer)
{
    float currentTime = (std::chrono::high_resolution_clock::now() - timer).count() * 0.000000001f;
    float currentFraction = chart->timeToFraction(currentTime);

    bool empty = true;
    for (size_t i = 0; i < chart->objs.size(); i++)
    {
        if (executed[i])
        {
            continue;
        }
        empty = false;

        if (chart->objs[i].type == bms::Obj::Type::NOTE)
        {
            float fractionDiff = chart->resolveSignatures(chart->objs[i].fraction) - currentFraction;
            SDL_Rect srcRect;
            SDL_FRect dstRect;
            srcRect.x = 0;
            srcRect.h = 10;
            dstRect.y = 480 * (1 - fractionDiff * speed) - 10;
            dstRect.h = 10;
            switch (chart->objs[i].note.player)
            {
            case 1:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    srcRect.y = 0;
                    srcRect.w = 60;
                    dstRect.w = 60;
                    dstRect.x = 0;
                    break;
                case 1:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 60;
                    break;
                case 2:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 100;
                    break;
                case 3:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 130;
                    break;
                case 4:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 170;
                    break;
                case 5:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 200;
                    break;
                case 8:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 240;
                    break;
                case 9:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 270;
                    break;
                }
                break;
            case 2:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    srcRect.y = 0;
                    srcRect.w = 60;
                    dstRect.w = 60;
                    dstRect.x = 580;
                    break;
                case 1:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 330;
                    break;
                case 2:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 370;
                    break;
                case 3:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 400;
                    break;
                case 4:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 440;
                    break;
                case 5:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 470;
                    break;
                case 8:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 510;
                    break;
                case 9:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 540;
                    break;
                }
                break;
            }
            SDL_RenderCopyF(renderer, notes, &srcRect, &dstRect);
        }
        else if (chart->objs[i].type == bms::Obj::Type::BOMB)
        {
            float fractionDiff = chart->resolveSignatures(chart->objs[i].fraction) - currentFraction;
            SDL_Rect srcRect;
            SDL_FRect dstRect;
            srcRect.x = 0;
            srcRect.h = 10;
            dstRect.y = 480 * (1 - fractionDiff * speed) - 10;
            dstRect.h = 10;
            switch (chart->objs[i].note.player)
            {
            case 1:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    srcRect.y = 0;
                    srcRect.w = 60;
                    dstRect.w = 60;
                    dstRect.x = 0;
                    break;
                case 1:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 60;
                    break;
                case 2:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 100;
                    break;
                case 3:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 130;
                    break;
                case 4:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 170;
                    break;
                case 5:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 200;
                    break;
                case 8:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 240;
                    break;
                case 9:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 270;
                    break;
                }
                break;
            case 2:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    srcRect.y = 0;
                    srcRect.w = 60;
                    dstRect.w = 60;
                    dstRect.x = 580;
                    break;
                case 1:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 330;
                    break;
                case 2:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 370;
                    break;
                case 3:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 400;
                    break;
                case 4:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 440;
                    break;
                case 5:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 470;
                    break;
                case 8:
                    srcRect.y = 20;
                    srcRect.w = 30;
                    dstRect.w = 30;
                    dstRect.x = 510;
                    break;
                case 9:
                    srcRect.y = 10;
                    srcRect.w = 40;
                    dstRect.w = 40;
                    dstRect.x = 540;
                    break;
                }
                break;
            }
            SDL_RenderCopyF(renderer, bombs, &srcRect, &dstRect);
        }

        if (chart->objs[i].time > currentTime)
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
    delete[] executed;
    delete chart;
    SDL_DestroyTexture(notes);
    SDL_DestroyTexture(bombs);
    audio::releaseAudio();
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