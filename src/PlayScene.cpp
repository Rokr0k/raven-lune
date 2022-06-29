#include "PlayScene.hpp"
#include "ListScene.hpp"
#include "file.hpp"
#include "font.hpp"
#include "audio.hpp"
#include "image.hpp"
#include "bga.hpp"
#include <algorithm>
#include <limits>

using namespace rl;

constexpr float judgeLine[][5] = {
    {0.008f, 0.024f, 0.040f, 0.200f, 1.000f},
    {0.015f, 0.030f, 0.060f, 0.200f, 1.000f},
    {0.018f, 0.040f, 0.100f, 0.200f, 1.000f},
    {0.021f, 0.060f, 0.120f, 0.200f, 1.000f},
};

void PlayScene::initialise()
{
    file::release();
    for (int i = 0; i < 1296; i++)
    {
        if (!chart->wavs[i].empty())
        {
            audio::loadAudio(i, chart->wavs[i]);
        }
    }
    for (int i = 0; i < 1296; i++)
    {
        if (!chart->bmps[i].empty())
        {
            bga::load(app->renderer, i, chart->bmps[i]);
        }
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

    noteCnt = std::count_if(chart->objs.begin(), chart->objs.end(), [](const bms::Obj &a)
                            { return a.type == bms::Obj::Type::NOTE && !a.note.end; });

    timer = SDL_GetTicks() + 5000;

    speed = 4;
    speedDisplay = font::renderText(app->renderer, std::to_string(speed / 4) + "." + std::to_string(speed * 10 / 4 % 10) + std::to_string(speed * 100 / 4 % 10));
    if (playMode == PlayMode::SINGLE)
    {
        int w, h;
        SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
        speedDisplayRect = {160, 460, (float)w / h * 20, 20};
    }
    else if (playMode == PlayMode::DUAL)
    {
        int w, h;
        SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
        speedDisplayRect = {320, 460, (float)w / h * 20, 20};
    }

    combo = 0;
    gauge = 20.0f;

    judgeTime = std::numeric_limits<float>().min();
    judgeDisplay = NULL;

    sectorIdx = 0;

    bgas[0] = -1;
    bgas[1] = -1;
    bgas[2] = 0;
    bgaRect = {(640 - (playMode == PlayMode::SINGLE ? 155 : 315) - 256) * 0.5f + (playMode == PlayMode::SINGLE ? 155 : 315), (480 - 256) * 0.5f, 256, 256};

    bpmDisplay = font::renderText(app->renderer, std::to_string((int)chart->sectors[sectorIdx].bpm).substr(0, 3));
    if (playMode == PlayMode::SINGLE)
    {
        int w, h;
        SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
        bpmDisplayRect = {400 - (float)w / h * 10, 450, (float)w / h * 20, 20};
    }
    if (playMode == PlayMode::DUAL)
    {
        int w, h;
        SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
        bpmDisplayRect = {480 - (float)w / h * 10, 450, (float)w / h * 20, 20};
    }

    for (int player = 1; player <= 2; player++)
    {
        for (int line = 1; line <= 9; line++)
        {
            pressed[std::make_pair(player, line)] = false;
        }
    }
}

void PlayScene::draw()
{
    float currentTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f;
    float currentFraction = chart->timeToFraction(currentTime);

    SDL_SetRenderDrawColor(app->renderer, 0x80, 0x80, 0xff, 0x80);
    if (pressed[std::make_pair(1, 6)])
    {
        SDL_FRect rect = {0, 0, 30, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 1)])
    {
        SDL_FRect rect = {30, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 2)])
    {
        SDL_FRect rect = {50, 0, 15, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 3)])
    {
        SDL_FRect rect = {65, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 4)])
    {
        SDL_FRect rect = {85, 0, 15, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 5)])
    {
        SDL_FRect rect = {100, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 8)])
    {
        SDL_FRect rect = {120, 0, 15, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[std::make_pair(1, 9)])
    {
        SDL_FRect rect = {135, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (playMode == PlayMode::DUAL)
    {
        if (pressed[std::make_pair(2, 1)])
        {
            SDL_FRect rect = {160, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 2)])
        {
            SDL_FRect rect = {180, 0, 15, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 3)])
        {
            SDL_FRect rect = {195, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 4)])
        {
            SDL_FRect rect = {215, 0, 15, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 5)])
        {
            SDL_FRect rect = {230, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 8)])
        {
            SDL_FRect rect = {250, 0, 15, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 9)])
        {
            SDL_FRect rect = {265, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[std::make_pair(2, 6)])
        {
            SDL_FRect rect = {285, 0, 30, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
    }

    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);

    SDL_RenderDrawLineF(app->renderer, 0, 0, 0, 480);
    SDL_RenderDrawLineF(app->renderer, 30, 0, 30, 480);
    SDL_RenderDrawLineF(app->renderer, 50, 0, 50, 480);
    SDL_RenderDrawLineF(app->renderer, 65, 0, 65, 480);
    SDL_RenderDrawLineF(app->renderer, 85, 0, 85, 480);
    SDL_RenderDrawLineF(app->renderer, 100, 0, 100, 480);
    SDL_RenderDrawLineF(app->renderer, 120, 0, 120, 480);
    SDL_RenderDrawLineF(app->renderer, 135, 0, 135, 480);
    SDL_RenderDrawLineF(app->renderer, 155, 0, 155, 480);
    if (playMode == PlayMode::DUAL)
    {
        SDL_FRect partition = {155, 0, 5, 480};
        SDL_RenderFillRectF(app->renderer, &partition);
        SDL_RenderDrawLineF(app->renderer, 160, 0, 160, 480);
        SDL_RenderDrawLineF(app->renderer, 180, 0, 180, 480);
        SDL_RenderDrawLineF(app->renderer, 195, 0, 195, 480);
        SDL_RenderDrawLineF(app->renderer, 215, 0, 215, 480);
        SDL_RenderDrawLineF(app->renderer, 230, 0, 230, 480);
        SDL_RenderDrawLineF(app->renderer, 250, 0, 250, 480);
        SDL_RenderDrawLineF(app->renderer, 265, 0, 265, 480);
        SDL_RenderDrawLineF(app->renderer, 285, 0, 285, 480);
        SDL_RenderDrawLineF(app->renderer, 315, 0, 315, 480);
    }

    float signature = 0;
    SDL_RenderDrawLineF(app->renderer, 0, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 155, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
    if (playMode == PlayMode::DUAL)
    {
        SDL_RenderDrawLineF(app->renderer, 160, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 315, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
    }
    for (int i = 0; i < 1000; i++)
    {
        signature += chart->signatures[i];
        SDL_RenderDrawLineF(app->renderer, 0, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 155, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
        if (playMode == PlayMode::DUAL)
        {
            SDL_RenderDrawLineF(app->renderer, 160, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 315, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
        }
    }

    bool empty = true;
    for (bms::Obj &obj : chart->objs)
    {
        if (obj.executed)
        {
            continue;
        }
        empty = false;

        if (obj.type == bms::Obj::Type::NOTE)
        {
            float fraction = chart->resolveSignatures(obj.fraction);
            float fractionDiff = fraction - currentFraction;
            SDL_Rect srcRect;
            SDL_FRect dstRect;
            dstRect.y = 480 * (1 - fractionDiff * speed * 0.25f) - 5;
            dstRect.h = 5;
            switch (obj.note.player)
            {
            case 1:
                switch (obj.note.line)
                {
                case 6:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0x00, 0x00, 0xff);
                    dstRect.w = 30;
                    dstRect.x = 0;
                    break;
                case 1:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 30;
                    break;
                case 2:
                    SDL_SetRenderDrawColor(app->renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 50;
                    break;
                case 3:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 65;
                    break;
                case 4:
                    SDL_SetRenderDrawColor(app->renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 85;
                    break;
                case 5:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 100;
                    break;
                case 8:
                    SDL_SetRenderDrawColor(app->renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 120;
                    break;
                case 9:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 135;
                    break;
                }
                break;
            case 2:
                switch (obj.note.line)
                {
                case 6:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0x00, 0x00, 0xff);
                    dstRect.w = 30;
                    dstRect.x = 285;
                    break;
                case 1:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 160;
                    break;
                case 2:
                    SDL_SetRenderDrawColor(app->renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 180;
                    break;
                case 3:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 195;
                    break;
                case 4:
                    SDL_SetRenderDrawColor(app->renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 215;
                    break;
                case 5:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 230;
                    break;
                case 8:
                    SDL_SetRenderDrawColor(app->renderer, 0x00, 0xff, 0xff, 0xff);
                    dstRect.w = 15;
                    dstRect.x = 250;
                    break;
                case 9:
                    SDL_SetRenderDrawColor(app->renderer, 0xff, 0xff, 0xff, 0xff);
                    dstRect.w = 20;
                    dstRect.x = 265;
                    break;
                }
                break;
            }
            if (obj.note.end)
            {
                const bms::Obj &note = obj;
                const bms::Obj &unt = *std::find_if(chart->objs.rbegin(), chart->objs.rend(), [&note](const bms::Obj &a)
                                                    { return a.type == bms::Obj::Type::NOTE && a.note.player == note.note.player && a.note.line == note.note.line && a.fraction < note.fraction; });
                dstRect.h = (fraction - chart->resolveSignatures(unt.fraction)) * speed * 0.25f * 480;
                dstRect.x += 5;
                dstRect.w -= 10;
            }
            SDL_RenderFillRectF(app->renderer, &dstRect);
        }
        else if (obj.type == bms::Obj::Type::BOMB)
        {
            float fractionDiff = chart->resolveSignatures(obj.fraction) - currentFraction;
            SDL_FRect dstRect;
            dstRect.y = 480 * (1 - fractionDiff * speed * 0.25f) - 5;
            dstRect.h = 5;
            SDL_SetRenderDrawColor(app->renderer, 0xff, 0x80, 0x80, 0xff);
            switch (obj.note.player)
            {
            case 1:
                switch (obj.note.line)
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
                switch (obj.note.line)
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
            SDL_RenderFillRectF(app->renderer, &dstRect);
        }

        if (obj.time > currentTime)
        {
            continue;
        }
        switch (obj.type)
        {
        case bms::Obj::Type::BGM:
            audio::playAudio(obj.bgm.key);
            obj.executed = true;
            break;
        case bms::Obj::Type::NOTE:
            if (automatic)
            {
                if (!obj.note.end)
                {
                    keydown(obj.note.player, obj.note.line);
                    std::vector<bms::Obj>::iterator iter = std::find_if(chart->objs.begin(), chart->objs.end(), [&obj](const bms::Obj &a)
                                                                        { return a.type == bms::Obj::Type::NOTE && a.note.player == obj.note.player && a.note.line == obj.note.line && a.fraction > obj.fraction && !a.executed; });
                    if (iter == chart->objs.end() || !iter->note.end)
                    {
                        keyupList.push_back(std::make_pair(currentTime + 0.1f, std::make_pair(obj.note.player, obj.note.line)));
                    }
                }
                else
                {
                    keyup(obj.note.player, obj.note.line);
                }
            }
            if (!obj.note.end)
            {
                if (currentTime - obj.time > judgeLine[chart->rank][3])
                {
                    judge(JudgeType::POOR);
                    obj.executed = true;
                }
            }
            else
            {
                obj.executed = true;
            }
            break;
        case bms::Obj::Type::BMP:
            if (obj.bmp.layer >= 0)
            {
                bgas[obj.bmp.layer] = obj.bmp.key;
            }
            else
            {
                bgas[2] = obj.bmp.key;
            }
            obj.executed = true;
            break;
        case bms::Obj::Type::BOMB:
            if (currentTime - obj.time > judgeLine[chart->rank][2])
            {
                obj.executed = true;
            }
        case bms::Obj::Type::INVISIBLE:
            obj.executed = true;
            break;
        }
    }

    if (automatic)
    {
        for (std::vector<std::pair<float, std::pair<int, int>>>::iterator i = keyupList.begin(), n = i; i != keyupList.end(); i = n)
        {
            ++n;
            if (i->first < currentTime)
            {
                keyup(i->second.first, i->second.second);
                n = keyupList.erase(i);
            }
        }
    }

    size_t prevSectorIdx = sectorIdx;
    while (sectorIdx + 1 < chart->sectors.size() && (chart->sectors[sectorIdx + 1].time < currentTime || chart->sectors[sectorIdx + 1].inclusive && chart->sectors[sectorIdx + 1].time == currentTime))
    {
        sectorIdx++;
    }
    if (sectorIdx != prevSectorIdx)
    {
        SDL_DestroyTexture(bpmDisplay);
        if (chart->sectors[sectorIdx].bpm > 0)
        {
            bpmDisplay = font::renderText(app->renderer, std::to_string((int)chart->sectors[sectorIdx].bpm).substr(0, 3));
        }
        else if (sectorIdx > 0)
        {
            bpmDisplay = font::renderText(app->renderer, std::to_string((int)chart->sectors[sectorIdx - 1].bpm).substr(0, 3));
        }
        else
        {
            bpmDisplay = font::renderText(app->renderer, "0");
        }
        if (playMode == PlayMode::SINGLE)
        {
            int w, h;
            SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
            bpmDisplayRect = {400 - (float)w / h * 10, 450, (float)w / h * 20, 20};
        }
        if (playMode == PlayMode::DUAL)
        {
            int w, h;
            SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
            bpmDisplayRect = {480 - (float)w / h * 10, 450, (float)w / h * 20, 20};
        }
    }
    SDL_RenderCopyF(app->renderer, bpmDisplay, NULL, &bpmDisplayRect);
    if (judgeDisplay)
    {
        SDL_RenderCopyF(app->renderer, judgeDisplay, NULL, &judgeDisplayRect[0]);
        if (playMode == PlayMode::DUAL)
        {
            SDL_RenderCopyF(app->renderer, judgeDisplay, NULL, &judgeDisplayRect[1]);
        }
        if (judgeTime < currentTime)
        {
            SDL_DestroyTexture(judgeDisplay);
            judgeDisplay = NULL;
        }
    }

    if (bgas[0] >= 0)
    {
        SDL_SetRenderDrawColor(app->renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRectF(app->renderer, &bgaRect);
        SDL_Texture *bga = bga::get(bgas[0]);
        SDL_RenderCopyF(app->renderer, bga, NULL, &bgaRect);
    }
    if (bgas[1] >= 0)
    {
        SDL_Texture *bga = bga::get(bgas[1]);
        SDL_RenderCopyF(app->renderer, bga, NULL, &bgaRect);
    }
    if (bgas[2] >= 0 && currentTime < judgeTime && (judgeType == JudgeType::POOR || judgeType == JudgeType::GPOOR))
    {
        SDL_Texture *bga = bga::get(bgas[2]);
        SDL_RenderCopyF(app->renderer, bga, NULL, &bgaRect);
    }

    SDL_RenderCopyF(app->renderer, speedDisplay, NULL, &speedDisplayRect);

    if (empty && !audio::isPlayingAudio())
    {
        app->changeScene(new ListScene(app));
    }
}

void PlayScene::release()
{
    SDL_DestroyTexture(bpmDisplay);
    SDL_DestroyTexture(speedDisplay);
    if (judgeDisplay)
    {
        SDL_DestroyTexture(judgeDisplay);
    }
    delete chart;
    audio::releaseAudio();
    file::initialise();
    bga::release();
}

void PlayScene::onkeydown(SDL_KeyboardEvent key)
{
    if (!key.repeat && !automatic)
    {
        switch (key.keysym.sym)
        {
        case SDLK_LSHIFT:
        case SDLK_LCTRL:
            keydown(1, 6);
            break;
        case SDLK_z:
            keydown(1, 1);
            break;
        case SDLK_s:
            keydown(1, 2);
            break;
        case SDLK_x:
            keydown(1, 3);
            break;
        case SDLK_d:
            keydown(1, 4);
            break;
        case SDLK_c:
            keydown(1, 5);
            break;
        case SDLK_f:
            keydown(1, 8);
            break;
        case SDLK_v:
            keydown(1, 9);
            break;
        case SDLK_RSHIFT:
        case SDLK_RCTRL:
            keydown(2, 6);
            break;
        case SDLK_m:
            keydown(2, 1);
            break;
        case SDLK_k:
            keydown(2, 2);
            break;
        case SDLK_COMMA:
            keydown(2, 3);
            break;
        case SDLK_l:
            keydown(2, 4);
            break;
        case SDLK_PERIOD:
            keydown(2, 5);
            break;
        case SDLK_SEMICOLON:
            keydown(2, 8);
            break;
        case SDLK_SLASH:
            keydown(2, 9);
            break;
        }
    }
    switch (key.keysym.sym)
    {
    case SDLK_UP:
        speed = std::min(speed + 1, 16);
        SDL_DestroyTexture(speedDisplay);
        speedDisplay = font::renderText(app->renderer, std::to_string(speed / 4) + "." + std::to_string(speed * 10 / 4 % 10) + std::to_string(speed * 100 / 4 % 10));
        if (playMode == PlayMode::SINGLE)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {160, 460, (float)w / h * 20, 20};
        }
        else if (playMode == PlayMode::DUAL)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {320, 460, (float)w / h * 20, 20};
        }
        break;
    case SDLK_DOWN:
        speed = std::max(speed - 1, 4);
        SDL_DestroyTexture(speedDisplay);
        speedDisplay = font::renderText(app->renderer, std::to_string(speed / 4) + "." + std::to_string(speed * 10 / 4 % 10) + std::to_string(speed * 100 / 4 % 10));
        if (playMode == PlayMode::SINGLE)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {160, 460, (float)w / h * 20, 20};
        }
        else if (playMode == PlayMode::DUAL)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {320, 460, (float)w / h * 20, 20};
        }
        break;
    }
}

void PlayScene::onkeyup(SDL_KeyboardEvent key)
{
    if (!automatic)
    {
        switch (key.keysym.sym)
        {
        case SDLK_LSHIFT:
        case SDLK_LCTRL:
            keyup(1, 6);
            break;
        case SDLK_z:
            keyup(1, 1);
            break;
        case SDLK_s:
            keyup(1, 2);
            break;
        case SDLK_x:
            keyup(1, 3);
            break;
        case SDLK_d:
            keyup(1, 4);
            break;
        case SDLK_c:
            keyup(1, 5);
            break;
        case SDLK_f:
            keyup(1, 8);
            break;
        case SDLK_v:
            keyup(1, 9);
            break;
        case SDLK_RSHIFT:
        case SDLK_RCTRL:
            keyup(2, 6);
            break;
        case SDLK_m:
            keyup(2, 1);
            break;
        case SDLK_k:
            keyup(2, 2);
            break;
        case SDLK_COMMA:
            keyup(2, 3);
            break;
        case SDLK_l:
            keyup(2, 4);
            break;
        case SDLK_PERIOD:
            keyup(2, 5);
            break;
        case SDLK_SEMICOLON:
            keyup(2, 8);
            break;
        case SDLK_SLASH:
            keyup(2, 9);
            break;
        }
    }
}

void PlayScene::keydown(int player, int line)
{
    pressed[std::make_pair(player, line)] = true;
    std::vector<bms::Obj>::iterator iter = std::find_if(chart->objs.begin(), chart->objs.end(), [player, line](const bms::Obj &a)
                                                        { return a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line && !a.note.end && !a.executed; });
    float currentTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f;
    if (iter != chart->objs.end())
    {
        bms::Obj &note = *iter;
        if (std::abs(note.time - currentTime) < judgeLine[chart->rank][0])
        {
            judge(JudgeType::JUST);
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][1])
        {
            judge(JudgeType::GREAT);
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][2])
        {
            judge(JudgeType::GOOD);
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][3])
        {
            judge(JudgeType::BAD);
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][4])
        {
            judge(JudgeType::GPOOR);
        }
        if (note.executed)
        {
            audio::playAudio(note.note.key);
        }
        else
        {
            iter = std::find_if(chart->objs.begin(), chart->objs.end(), [player, line](const bms::Obj &a)
                                { return a.type == bms::Obj::Type::BOMB && a.misc.player == player && a.misc.line == line && !a.executed; });
            if (iter != chart->objs.end())
            {
                bms::Obj &bomb = *iter;
                if (std::abs(note.time - currentTime) < judgeLine[chart->rank][2])
                {
                    bomb.executed = true;
                }
                else
                {
                    goto none;
                }
            }
            else
            {
            none:
                std::vector<bms::Obj>::reverse_iterator riter = std::find_if(chart->objs.rbegin(), chart->objs.rend(), [player, line](const bms::Obj &a)
                                                                             { return (a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line || a.type == bms::Obj::Type::INVISIBLE && a.misc.player == player && a.misc.line == line) && a.executed; });
                if (riter != chart->objs.rend())
                {
                    switch (riter->type)
                    {
                    case bms::Obj::Type::NOTE:
                        audio::playAudio(riter->note.key);
                        break;
                    case bms::Obj::Type::INVISIBLE:
                        audio::playAudio(riter->misc.key);
                        break;
                    }
                }
            }
        }
    }
}

void PlayScene::keyup(int player, int line)
{
    pressed[std::make_pair(player, line)] = false;
    std::vector<bms::Obj>::iterator iter = std::find_if(chart->objs.begin(), chart->objs.end(), [&player, &line](const bms::Obj &a)
                                                        { return a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line && !a.executed; });
    float currentTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f;
    if (iter != chart->objs.end() && iter->note.end)
    {
        bms::Obj &note = *iter;
        if (note.time - currentTime > judgeLine[chart->rank][2])
        {
            judge(JudgeType::POOR);
        }
        note.executed = true;
    }
}

void PlayScene::judge(JudgeType j)
{
    judgeType = j;
    judgeTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f + 1.0f;
    if (judgeDisplay)
    {
        SDL_DestroyTexture(judgeDisplay);
    }
    switch (j)
    {
    case JudgeType::JUST:
        combo++;
        gauge = std::min(gauge + chart->total / noteCnt, 100.0f);
        judgeDisplay = font::renderText(app->renderer, "GREAT " + std::to_string(combo));
        SDL_SetTextureColorMod(judgeDisplay, 0xcc, 0xcc, 0xcc);
        break;
    case JudgeType::GREAT:
        combo++;
        gauge = std::min(gauge + chart->total / noteCnt, 100.0f);
        judgeDisplay = font::renderText(app->renderer, "GREAT " + std::to_string(combo));
        SDL_SetTextureColorMod(judgeDisplay, 0xff, 0xd7, 0x00);
        break;
    case JudgeType::GOOD:
        combo++;
        gauge = std::min(gauge + chart->total / noteCnt * 0.5f, 100.0f);
        judgeDisplay = font::renderText(app->renderer, "GOOD " + std::to_string(combo));
        SDL_SetTextureColorMod(judgeDisplay, 0xad, 0xff, 0x2f);
        break;
    case JudgeType::BAD:
        combo = 0;
        gauge = std::max(gauge - 4, 0.0f);
        judgeDisplay = font::renderText(app->renderer, "BAD");
        SDL_SetTextureColorMod(judgeDisplay, 0xee, 0x82, 0xee);
        break;
    case JudgeType::POOR:
        combo = 0;
        gauge = std::max(gauge - 6, 0.0f);
        judgeDisplay = font::renderText(app->renderer, "POOR");
        SDL_SetTextureColorMod(judgeDisplay, 0xdc, 0x14, 0x3c);
        break;
    case JudgeType::GPOOR:
        gauge = std::max(gauge - 2, 0.0f);
        judgeDisplay = font::renderText(app->renderer, "POOR");
        SDL_SetTextureColorMod(judgeDisplay, 0xdc, 0x14, 0x3c);
        break;
    }
    int w, h;
    SDL_QueryTexture(judgeDisplay, NULL, NULL, &w, &h);
    judgeDisplayRect[0] = {77.5f - (float)w / h * 20, 335, (float)w / h * 40, 40};
    if (playMode == PlayMode::DUAL)
    {
        judgeDisplayRect[1] = {237.5f - (float)w / h * 20, 335, (float)w / h * 40, 40};
    }
}