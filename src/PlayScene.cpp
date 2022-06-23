#include "PlayScene.hpp"
#include "ListScene.hpp"
#include "file.hpp"
#include "audio.hpp"
#include "image.hpp"
#include <algorithm>

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

    if (std::find_if(chart->objs.begin(), chart->objs.end(), [](const bms::Obj &a)
                     { return a.type == bms::Obj::Type::NOTE && a.note.player == 2 || (a.type == bms::Obj::Type::BOMB || a.type == bms::Obj::Type::INVISIBLE) && a.misc.player == 2; }) == chart->objs.end())
    {
        playMode = PlayMode::SINGLE;
    }
    else
    {
        playMode = PlayMode::DUAL;
    }

    timer = std::chrono::high_resolution_clock::now();

    speed = 2;
}

void PlayScene::draw(SDL_Renderer *renderer)
{
    float currentTime = (std::chrono::high_resolution_clock::now() - timer).count() * 0.000000001f;
    float currentFraction = chart->timeToFraction(currentTime);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

    SDL_RenderDrawLineF(renderer, 0, 0, 0, 480);
    SDL_RenderDrawLineF(renderer, 30, 0, 30, 480);
    SDL_RenderDrawLineF(renderer, 50, 0, 50, 480);
    SDL_RenderDrawLineF(renderer, 65, 0, 65, 480);
    SDL_RenderDrawLineF(renderer, 85, 0, 85, 480);
    SDL_RenderDrawLineF(renderer, 100, 0, 100, 480);
    SDL_RenderDrawLineF(renderer, 120, 0, 120, 480);
    SDL_RenderDrawLineF(renderer, 135, 0, 135, 480);
    SDL_RenderDrawLineF(renderer, 155, 0, 155, 480);
    if (playMode == PlayMode::DUAL)
    {
        SDL_FRect partition = {155, 0, 5, 480};
        SDL_RenderFillRectF(renderer, &partition);
        SDL_RenderDrawLineF(renderer, 160, 0, 160, 480);
        SDL_RenderDrawLineF(renderer, 180, 0, 180, 480);
        SDL_RenderDrawLineF(renderer, 195, 0, 195, 480);
        SDL_RenderDrawLineF(renderer, 215, 0, 215, 480);
        SDL_RenderDrawLineF(renderer, 230, 0, 230, 480);
        SDL_RenderDrawLineF(renderer, 250, 0, 250, 480);
        SDL_RenderDrawLineF(renderer, 265, 0, 265, 480);
        SDL_RenderDrawLineF(renderer, 285, 0, 285, 480);
        SDL_RenderDrawLineF(renderer, 315, 0, 315, 480);
    }

    float signature = 0;
    SDL_RenderDrawLineF(renderer, 0, 480 * (1 - (signature - currentFraction) * speed), 155, 480 * (1 - (signature - currentFraction) * speed));
    if (playMode == PlayMode::DUAL)
    {
        SDL_RenderDrawLineF(renderer, 160, 480 * (1 - (signature - currentFraction) * speed), 315, 480 * (1 - (signature - currentFraction) * speed));
    }
    for (int i = 0; i < 1000; i++)
    {
        signature += chart->signatures[i];
        SDL_RenderDrawLineF(renderer, 0, 480 * (1 - (signature - currentFraction) * speed), 155, 480 * (1 - (signature - currentFraction) * speed));
        if (playMode == PlayMode::DUAL)
        {
            SDL_RenderDrawLineF(renderer, 160, 480 * (1 - (signature - currentFraction) * speed), 315, 480 * (1 - (signature - currentFraction) * speed));
        }
    }

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
            float fraction = chart->resolveSignatures(chart->objs[i].fraction);
            float fractionDiff = fraction - currentFraction;
            SDL_Rect srcRect;
            SDL_FRect dstRect;
            dstRect.y = 480 * (1 - fractionDiff * speed) - 5;
            dstRect.h = 5;
            switch (chart->objs[i].note.player)
            {
            case 1:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
                    dstRect.w = 30;
                    dstRect.x = 0;
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 30;
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 50;
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 65;
                    break;
                case 4:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 85;
                    break;
                case 5:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 100;
                    break;
                case 8:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 120;
                    break;
                case 9:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 135;
                    break;
                }
                break;
            case 2:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
                    dstRect.w = 30;
                    dstRect.x = 285;
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 160;
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 180;
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 195;
                    break;
                case 4:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 215;
                    break;
                case 5:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 230;
                    break;
                case 8:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 250;
                    break;
                case 9:
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 265;
                    break;
                }
                break;
            }
            if (chart->objs[i].note.end)
            {
                const bms::Obj &note = chart->objs[i];
                const bms::Obj &unt = *std::find_if(chart->objs.rbegin(), chart->objs.rend(), [&note](const bms::Obj &a)
                                                    { return a.type == bms::Obj::Type::NOTE && a.note.player == note.note.player && a.note.line == note.note.line && a.fraction < note.fraction; });
                dstRect.h = (fraction - chart->resolveSignatures(unt.fraction)) * speed * 480;
                dstRect.x += 5;
                dstRect.w -= 10;
            }
            SDL_RenderFillRectF(renderer, &dstRect);
        }
        else if (chart->objs[i].type == bms::Obj::Type::BOMB)
        {
            float fractionDiff = chart->resolveSignatures(chart->objs[i].fraction) - currentFraction;
            SDL_FRect dstRect;
            dstRect.y = 480 * (1 - fractionDiff * speed) - 5;
            dstRect.h = 5;
            SDL_SetRenderDrawColor(renderer, 0xff, 0x80, 0x80, 0xff);
            switch (chart->objs[i].note.player)
            {
            case 1:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    dstRect.w = 30;
                    dstRect.x = 0;
                    break;
                case 1:
                    dstRect.w = 20;
                    dstRect.x = 30;
                    break;
                case 2:
                    dstRect.w = 15;
                    dstRect.x = 50;
                    break;
                case 3:
                    dstRect.w = 20;
                    dstRect.x = 65;
                    break;
                case 4:
                    dstRect.w = 15;
                    dstRect.x = 85;
                    break;
                case 5:
                    dstRect.w = 20;
                    dstRect.x = 100;
                    break;
                case 8:
                    dstRect.w = 15;
                    dstRect.x = 120;
                    break;
                case 9:
                    dstRect.w = 20;
                    dstRect.x = 135;
                    break;
                }
                break;
            case 2:
                switch (chart->objs[i].note.line)
                {
                case 6:
                    dstRect.w = 30;
                    dstRect.x = 285;
                    break;
                case 1:
                    dstRect.w = 20;
                    dstRect.x = 160;
                    break;
                case 2:
                    dstRect.w = 15;
                    dstRect.x = 180;
                    break;
                case 3:
                    dstRect.w = 20;
                    dstRect.x = 195;
                    break;
                case 4:
                    dstRect.w = 15;
                    dstRect.x = 215;
                    break;
                case 5:
                    dstRect.w = 20;
                    dstRect.x = 230;
                    break;
                case 8:
                    dstRect.w = 15;
                    dstRect.x = 250;
                    break;
                case 9:
                    dstRect.w = 20;
                    dstRect.x = 265;
                    break;
                }
                break;
            }
            SDL_RenderFillRectF(renderer, &dstRect);
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