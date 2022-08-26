#include "PlayScene.hpp"
#include "ListScene.hpp"
#include "ResultScene.hpp"
#include "file.hpp"
#include "font.hpp"
#include "audio.hpp"
#include "image.hpp"
#include "bga.hpp"
#include "keys.hpp"
#include <algorithm>
#include <limits>

using namespace rl;

constexpr float judgeLine[][5] = {
    {0.008f, 0.024f, 0.040f, 0.200f, 1.000f},
    {0.015f, 0.030f, 0.060f, 0.200f, 1.000f},
    {0.018f, 0.040f, 0.100f, 0.200f, 1.000f},
    {0.021f, 0.060f, 0.120f, 0.200f, 1.000f},
};

void PlayScene::initialise(App *app)
{
    Scene::initialise(app);
    for (int i = 0; i < 1296; i++)
    {
        if (!chart->wavs[i].empty())
        {
            audio::loadAudio(i, chart->wavs[i]);
        }
        if (!chart->bmps[i].empty())
        {
            bga::load(i, chart->bmps[i]);
        }
    }

    objs.assign(chart->objs.begin(), chart->objs.end());

    noteCnt = std::count_if(objs.begin(), objs.end(), [](const bms::Obj &a)
                            { return a.type == bms::Obj::Type::NOTE && !a.note.end; });

    timer = SDL_GetTicks() + 5000;

    font::loadSize(20);
    font::loadSize(40);

    speed = 4;
    speedDisplay = font::renderText(app->renderer, std::to_string(speed / 4) + "." + std::to_string(speed * 10 / 4 % 10) + std::to_string(speed * 100 / 4 % 10), 20);
    if (chart->type == bms::Chart::Type::Single)
    {
        int w, h;
        SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
        speedDisplayRect = {160, 460, (float)w / h * 20, 20};
    }
    else if (chart->type == bms::Chart::Type::Dual)
    {
        int w, h;
        SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
        speedDisplayRect = {320, 460, (float)w / h * 20, 20};
    }

    combo = 0;
    maxCombo = 0;
    gauge = 20.0f;
    judgeCount[0] = judgeCount[1] = judgeCount[2] = judgeCount[3] = judgeCount[4] = 0;

    judgeTime = std::numeric_limits<float>().min();
    judgeDisplay = NULL;

    sectorIdx = 0;

    bgas[0] = -1;
    bgas[1] = -1;
    bgas[2] = 0;
    bgaRect = {(640 - (chart->type == bms::Chart::Type::Single ? 155 : 315) - 256) * 0.5f + (chart->type == bms::Chart::Type::Single ? 155 : 315), (480 - 256) * 0.5f, 256, 256};

    bpmDisplay = font::renderText(app->renderer, std::to_string((int)chart->sectors[sectorIdx].bpm).substr(0, 3), 20);
    if (chart->type == bms::Chart::Type::Single)
    {
        int w, h;
        SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
        bpmDisplayRect = {400 - (float)w / h * 10, 450, (float)w / h * 20, 20};
    }
    if (chart->type == bms::Chart::Type::Dual)
    {
        int w, h;
        SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
        bpmDisplayRect = {480 - (float)w / h * 10, 450, (float)w / h * 20, 20};
    }

    for (int player = 1; player <= 2; player++)
    {
        for (int line = 1; line <= 9; line++)
        {
            pressed[{player, line}] = false;
        }
    }

    if (chart->type == bms::Chart::Type::Single)
    {
        gaugeDisplayRect[0] = {165, 430 - 380 * std::min(gauge, 80.0f) * 0.01f, 10, 380 * std::min(gauge, 80.0f) * 0.01f};
        gaugeDisplayRect[1] = {165, 126 - 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f, 10, 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f};
    }
    else if (chart->type == bms::Chart::Type::Dual)
    {
        gaugeDisplayRect[0] = {325, 430 - 380 * std::min(gauge, 80.0f) * 0.01f, 10, 380 * std::min(gauge, 80.0f) * 0.01f};
        gaugeDisplayRect[1] = {325, 126 - 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f, 10, 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f};
    }

    autoplayDisplay = NULL;
    if (automatic)
    {
        int w, h;
        autoplayDisplay = font::renderText(app->renderer, "autoplay", 20);
        SDL_QueryTexture(autoplayDisplay, NULL, NULL, &w, &h);
        autoplayDisplayRect = {77.5f - (float)w / h * 10, 360, (float)w / h * 20, 20};
    }
}

void PlayScene::draw()
{
    float currentTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f;
    float currentFraction = chart->timeToFraction(currentTime);

    SDL_SetRenderDrawColor(app->renderer, 0x80, 0x80, 0xff, 0x80);
    if (pressed[{1, 6}])
    {
        SDL_FRect rect = {0, 0, 30, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 1}])
    {
        SDL_FRect rect = {30, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 2}])
    {
        SDL_FRect rect = {50, 0, 15, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 3}])
    {
        SDL_FRect rect = {65, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 4}])
    {
        SDL_FRect rect = {85, 0, 15, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 5}])
    {
        SDL_FRect rect = {100, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 8}])
    {
        for (int i = 0; i < 1296; i++)
        {
        }
        SDL_FRect rect = {120, 0, 15, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (pressed[{1, 9}])
    {
        SDL_FRect rect = {135, 0, 20, 480};
        SDL_RenderFillRectF(app->renderer, &rect);
    }
    if (chart->type == bms::Chart::Type::Dual)
    {
        if (pressed[{2, 1}])
        {
            SDL_FRect rect = {160, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 2}])
        {
            SDL_FRect rect = {180, 0, 15, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 3}])
        {
            SDL_FRect rect = {195, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 4}])
        {
            SDL_FRect rect = {215, 0, 15, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 5}])
        {
            SDL_FRect rect = {230, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 8}])
        {
            SDL_FRect rect = {250, 0, 15, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 9}])
        {
            SDL_FRect rect = {265, 0, 20, 480};
            SDL_RenderFillRectF(app->renderer, &rect);
        }
        if (pressed[{2, 6}])
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
    if (chart->type == bms::Chart::Type::Dual)
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
    if (chart->type == bms::Chart::Type::Dual)
    {
        SDL_RenderDrawLineF(app->renderer, 160, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 315, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
    }
    for (int i = 0; i < 1000; i++)
    {
        signature += chart->signatures[i];
        SDL_RenderDrawLineF(app->renderer, 0, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 155, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
        if (chart->type == bms::Chart::Type::Dual)
        {
            SDL_RenderDrawLineF(app->renderer, 160, 480 * (1 - (signature - currentFraction) * speed * 0.25f), 315, 480 * (1 - (signature - currentFraction) * speed * 0.25f));
        }
    }

    bool empty = true;
    for (Obj &obj : objs)
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
                const bms::Obj &unt = *std::find_if(objs.rbegin(), objs.rend(), [&note](const bms::Obj &a)
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
                    std::vector<Obj>::iterator iter = std::find_if(objs.begin(), objs.end(), [&obj](const Obj &a)
                                                                   { return a.type == bms::Obj::Type::NOTE && a.note.player == obj.note.player && a.note.line == obj.note.line && a.fraction > obj.fraction && !a.executed; });
                    if (iter == objs.end() || !iter->note.end)
                    {
                        float a = 0.1f;
                        if (iter != objs.end())
                        {
                            a = std::min(a, (iter->time - obj.time) / 2.0f);
                        }
                        keyupList.push_back({currentTime + a, {obj.note.player, obj.note.line}});
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
                    std::vector<Obj>::iterator iter = std::find_if(objs.begin(), objs.end(), [&obj](const Obj &a)
                                                                   { return a.type == bms::Obj::Type::NOTE && a.note.player == obj.note.player && a.note.line == obj.note.line && a.fraction > obj.fraction && !a.executed; });
                    if (iter != objs.end() && iter->note.end)
                    {
                        iter->executed = true;
                    }
                }
            }
            else
            {
                if (suspendedJudge.find({obj.note.player, obj.note.line}) != suspendedJudge.end() && suspendedJudge[{obj.note.player, obj.note.line}] != JudgeType::NONE)
                {
                    judge(suspendedJudge[{obj.note.player, obj.note.line}]);
                }
                obj.executed = true;
            }
            break;
        case bms::Obj::Type::BMP:
            if (obj.bmp.layer >= 0)
            {
                bgas[obj.bmp.layer] = obj.bmp.key;
                bgaStarted[obj.bmp.layer] = obj.time;
            }
            else
            {
                bgas[2] = obj.bmp.key;
                bgaStarted[2] = obj.time;
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
            bpmDisplay = font::renderText(app->renderer, std::to_string((int)chart->sectors[sectorIdx].bpm).substr(0, 3), 20);
        }
        else if (sectorIdx > 0)
        {
            bpmDisplay = font::renderText(app->renderer, std::to_string((int)chart->sectors[sectorIdx - 1].bpm).substr(0, 3), 20);
        }
        else
        {
            bpmDisplay = font::renderText(app->renderer, "0", 20);
        }
        if (chart->type == bms::Chart::Type::Single)
        {
            int w, h;
            SDL_QueryTexture(bpmDisplay, NULL, NULL, &w, &h);
            bpmDisplayRect = {400 - (float)w / h * 10, 450, (float)w / h * 20, 20};
        }
        else if (chart->type == bms::Chart::Type::Dual)
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
        if (chart->type == bms::Chart::Type::Dual)
        {
            SDL_RenderCopyF(app->renderer, judgeDisplay, NULL, &judgeDisplayRect[1]);
        }
        if (judgeTime < currentTime)
        {
            SDL_DestroyTexture(judgeDisplay);
            judgeDisplay = NULL;
        }
    }
    if (automatic)
    {
        SDL_RenderCopyF(app->renderer, autoplayDisplay, NULL, &autoplayDisplayRect);
    }

    if (bgas[0] >= 0)
    {
        SDL_SetRenderDrawColor(app->renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRectF(app->renderer, &bgaRect);
        SDL_Surface *bga = bga::get(bgas[0], currentTime - bgaStarted[0]);
        SDL_Texture *bgaT = SDL_CreateTextureFromSurface(app->renderer, bga);
        SDL_RenderCopyF(app->renderer, bgaT, NULL, &bgaRect);
        SDL_DestroyTexture(bgaT);
    }
    if (bgas[1] >= 0)
    {
        SDL_Surface *bga = bga::get(bgas[1], currentTime - bgaStarted[1]);
        SDL_Texture *bgaT = SDL_CreateTextureFromSurface(app->renderer, bga);
        SDL_RenderCopyF(app->renderer, bgaT, NULL, &bgaRect);
        SDL_DestroyTexture(bgaT);
    }
    if (bgas[2] >= 0 && currentTime < judgeTime && (judgeType == JudgeType::POOR || judgeType == JudgeType::GPOOR))
    {
        SDL_Surface *bga = bga::get(bgas[2], currentTime - bgaStarted[2]);
        SDL_Texture *bgaT = SDL_CreateTextureFromSurface(app->renderer, bga);
        SDL_RenderCopyF(app->renderer, bgaT, NULL, &bgaRect);
        SDL_DestroyTexture(bgaT);
    }

    SDL_SetRenderDrawColor(app->renderer, 0x33, 0xff, 0xff, 0xff);
    SDL_RenderFillRectF(app->renderer, &gaugeDisplayRect[0]);
    SDL_SetRenderDrawColor(app->renderer, 0xff, 0x33, 0x33, 0xff);
    SDL_RenderFillRectF(app->renderer, &gaugeDisplayRect[1]);

    SDL_RenderCopyF(app->renderer, speedDisplay, NULL, &speedDisplayRect);

    if (empty && !audio::isPlayingAudio())
    {
        if (automatic)
        {
            app->changeScene(new ListScene());
        }
        else
        {
            app->changeScene(new ResultScene(ResultScene::Result{gauge > 80, {judgeCount[0], judgeCount[1], judgeCount[2], judgeCount[3], judgeCount[4]}, maxCombo, noteCnt}));
        }
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
    if (autoplayDisplay)
    {
        SDL_DestroyTexture(autoplayDisplay);
    }
    font::unloadSize(20);
    font::unloadSize(40);
    audio::releaseAudio();
    bga::release();
}

void PlayScene::onkeydown(SDL_KeyboardEvent key)
{
    if (!key.repeat && !automatic)
    {
        if (chart->type == bms::Chart::Type::Single)
        {
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_SU) || key.keysym.sym == keys::getKey(keys::KeysType::S_SD))
            {
                keydown(1, 6);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_1))
            {
                keydown(1, 1);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_2))
            {
                keydown(1, 2);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_3))
            {
                keydown(1, 3);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_4))
            {
                keydown(1, 4);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_5))
            {
                keydown(1, 5);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_8))
            {
                keydown(1, 8);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_9))
            {
                keydown(1, 9);
            }
        }
        else if (chart->type == bms::Chart::Type::Dual)
        {
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_SU) || key.keysym.sym == keys::getKey(keys::KeysType::D1_SD))
            {
                keydown(1, 6);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_1))
            {
                keydown(1, 1);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_2))
            {
                keydown(1, 2);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_3))
            {
                keydown(1, 3);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_4))
            {
                keydown(1, 4);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_5))
            {
                keydown(1, 5);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_8))
            {
                keydown(1, 8);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_9))
            {
                keydown(1, 9);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_SU) || key.keysym.sym == keys::getKey(keys::KeysType::D2_SD))
            {
                keydown(2, 6);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_1))
            {
                keydown(2, 1);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_2))
            {
                keydown(2, 2);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_3))
            {
                keydown(2, 3);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_4))
            {
                keydown(2, 4);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_5))
            {
                keydown(2, 5);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_8))
            {
                keydown(2, 8);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_9))
            {
                keydown(2, 9);
            }
        }
    }
    switch (key.keysym.sym)
    {
    case SDLK_UP:
        speed = std::min(speed + 1, 40);
        SDL_DestroyTexture(speedDisplay);
        speedDisplay = font::renderText(app->renderer, std::to_string(speed / 4) + "." + std::to_string(speed * 10 / 4 % 10) + std::to_string(speed * 100 / 4 % 10), 20);
        if (chart->type == bms::Chart::Type::Single)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {160, 460, (float)w / h * 20, 20};
        }
        else if (chart->type == bms::Chart::Type::Dual)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {320, 460, (float)w / h * 20, 20};
        }
        break;
    case SDLK_DOWN:
        speed = std::max(speed - 1, 1);
        SDL_DestroyTexture(speedDisplay);
        speedDisplay = font::renderText(app->renderer, std::to_string(speed / 4) + "." + std::to_string(speed * 10 / 4 % 10) + std::to_string(speed * 100 / 4 % 10), 20);
        if (chart->type == bms::Chart::Type::Single)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {160, 460, (float)w / h * 20, 20};
        }
        else if (chart->type == bms::Chart::Type::Dual)
        {
            int w, h;
            SDL_QueryTexture(speedDisplay, NULL, NULL, &w, &h);
            speedDisplayRect = {320, 460, (float)w / h * 20, 20};
        }
        break;
    case SDLK_ESCAPE:
        app->changeScene(new ListScene());
        break;
    }
}

void PlayScene::onkeyup(SDL_KeyboardEvent key)
{
    if (!automatic)
    {
        if (chart->type == bms::Chart::Type::Single)
        {
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_SU) || key.keysym.sym == keys::getKey(keys::KeysType::S_SD))
            {
                keyup(1, 6);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_1))
            {
                keyup(1, 1);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_2))
            {
                keyup(1, 2);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_3))
            {
                keyup(1, 3);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_4))
            {
                keyup(1, 4);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_5))
            {
                keyup(1, 5);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_8))
            {
                keyup(1, 8);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::S_9))
            {
                keyup(1, 9);
            }
        }
        else if (chart->type == bms::Chart::Type::Dual)
        {
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_SU) || key.keysym.sym == keys::getKey(keys::KeysType::D1_SD))
            {
                keyup(1, 6);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_1))
            {
                keyup(1, 1);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_2))
            {
                keyup(1, 2);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_3))
            {
                keyup(1, 3);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_4))
            {
                keyup(1, 4);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_5))
            {
                keyup(1, 5);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_8))
            {
                keyup(1, 8);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D1_9))
            {
                keyup(1, 9);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_SU) || key.keysym.sym == keys::getKey(keys::KeysType::D2_SD))
            {
                keyup(2, 6);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_1))
            {
                keyup(2, 1);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_2))
            {
                keyup(2, 2);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_3))
            {
                keyup(2, 3);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_4))
            {
                keyup(2, 4);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_5))
            {
                keyup(2, 5);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_8))
            {
                keyup(2, 8);
            }
            if (key.keysym.sym == keys::getKey(keys::KeysType::D2_9))
            {
                keyup(2, 9);
            }
        }
    }
}

void PlayScene::keydown(int player, int line)
{
    pressed[{player, line}] = true;
    std::vector<Obj>::iterator iter = std::find_if(objs.begin(), objs.end(), [player, line](const Obj &a)
                                                   { return a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line && !a.note.end && !a.executed; });
    float currentTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f;
    if (iter != objs.end())
    {
        Obj &note = *iter;
        JudgeType j = JudgeType::NONE;
        if (std::abs(note.time - currentTime) < judgeLine[chart->rank][0])
        {
            j = JudgeType::JUST;
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][1])
        {
            j = JudgeType::GREAT;
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][2])
        {
            j = JudgeType::GOOD;
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][3])
        {
            j = JudgeType::BAD;
            note.executed = true;
        }
        else if (std::abs(note.time - currentTime) < judgeLine[chart->rank][4])
        {
            j = JudgeType::GPOOR;
        }
        if (j != JudgeType::NONE)
        {
            std::vector<Obj>::iterator next = std::find_if(iter + 1, objs.end(), [player, line](const Obj &a)
                                                           { return a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line && !a.executed; });
            if (next != objs.end() && next->note.end)
            {
                suspendedJudge[{player, line}] = j;
            }
            else
            {
                judge(j);
            }
        }
        if (note.executed)
        {
            audio::playAudio(note.note.key);
        }
        else
        {
            iter = std::find_if(objs.begin(), objs.end(), [player, line](const Obj &a)
                                { return a.type == bms::Obj::Type::BOMB && a.misc.player == player && a.misc.line == line && !a.executed; });
            if (iter != objs.end())
            {
                Obj &bomb = *iter;
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
                std::vector<Obj>::reverse_iterator riter = std::find_if(objs.rbegin(), objs.rend(), [player, line](const Obj &a)
                                                                        { return (a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line || a.type == bms::Obj::Type::INVISIBLE && a.misc.player == player && a.misc.line == line) && a.executed; });
                if (riter != objs.rend())
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
    pressed[{player, line}] = false;
    std::vector<Obj>::iterator iter = std::find_if(objs.begin(), objs.end(), [&player, &line](const Obj &a)
                                                   { return a.type == bms::Obj::Type::NOTE && a.note.player == player && a.note.line == line && !a.executed; });
    float currentTime = ((float)SDL_GetTicks() - (float)timer) * 0.001f;
    if (iter != objs.end() && iter->note.end)
    {
        Obj &note = *iter;
        if (note.time - currentTime > judgeLine[chart->rank][2])
        {
            judge(JudgeType::POOR);
            std::vector<Obj>::reverse_iterator start = std::find_if(objs.rbegin(), objs.rend(), [&note](const Obj &a)
                                                                    { return a.type == bms::Obj::Type::NOTE && a.note.player == note.note.player && a.note.line == note.note.line && a.time < note.time; });
            if (start != objs.rend())
                audio::stopAudio(start->note.key);
        }
        else if (suspendedJudge.find({note.note.player, note.note.line}) != suspendedJudge.end() && suspendedJudge[{note.note.player, note.note.line}] != JudgeType::NONE)
        {
            judge(suspendedJudge[{note.note.player, note.note.line}]);
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
        judgeDisplay = font::renderText(app->renderer, "GREAT " + std::to_string(combo), 40);
        SDL_SetTextureColorMod(judgeDisplay, 0xcc, 0xcc, 0xcc);
        judgeCount[0]++;
        break;
    case JudgeType::GREAT:
        combo++;
        gauge = std::min(gauge + chart->total / noteCnt, 100.0f);
        judgeDisplay = font::renderText(app->renderer, "GREAT " + std::to_string(combo), 40);
        SDL_SetTextureColorMod(judgeDisplay, 0xff, 0xd7, 0x00);
        judgeCount[1]++;
        break;
    case JudgeType::GOOD:
        combo++;
        gauge = std::min(gauge + chart->total / noteCnt * 0.5f, 100.0f);
        judgeDisplay = font::renderText(app->renderer, "GOOD " + std::to_string(combo), 40);
        SDL_SetTextureColorMod(judgeDisplay, 0xad, 0xff, 0x2f);
        judgeCount[2]++;
        break;
    case JudgeType::BAD:
        combo = 0;
        gauge = std::max(gauge - 4, 0.0f);
        judgeDisplay = font::renderText(app->renderer, "BAD", 40);
        SDL_SetTextureColorMod(judgeDisplay, 0xee, 0x82, 0xee);
        judgeCount[3]++;
        break;
    case JudgeType::POOR:
        combo = 0;
        gauge = std::max(gauge - 6, 0.0f);
        judgeDisplay = font::renderText(app->renderer, "POOR", 40);
        SDL_SetTextureColorMod(judgeDisplay, 0xdc, 0x14, 0x3c);
        judgeCount[4]++;
        break;
    case JudgeType::GPOOR:
        gauge = std::max(gauge - 2, 0.0f);
        judgeDisplay = font::renderText(app->renderer, "POOR", 40);
        SDL_SetTextureColorMod(judgeDisplay, 0xdc, 0x14, 0x3c);
        judgeCount[4]++;
        break;
    }
    maxCombo = std::max(maxCombo, combo);
    int w, h;
    SDL_QueryTexture(judgeDisplay, NULL, NULL, &w, &h);
    judgeDisplayRect[0] = {77.5f - (float)w / h * 20, 335, (float)w / h * 40, 40};
    if (chart->type == bms::Chart::Type::Dual)
    {
        judgeDisplayRect[1] = {237.5f - (float)w / h * 20, 335, (float)w / h * 40, 40};
    }

    if (chart->type == bms::Chart::Type::Single)
    {
        gaugeDisplayRect[0] = {165, 430 - 380 * std::min(gauge, 80.0f) * 0.01f, 10, 380 * std::min(gauge, 80.0f) * 0.01f};
        gaugeDisplayRect[1] = {165, 126 - 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f, 10, 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f};
    }
    else if (chart->type == bms::Chart::Type::Dual)
    {
        gaugeDisplayRect[0] = {325, 430 - 380 * std::min(gauge, 80.0f) * 0.01f, 10, 380 * std::min(gauge, 80.0f) * 0.01f};
        gaugeDisplayRect[1] = {325, 126 - 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f, 10, 380 * (std::max(gauge, 80.0f) - 80.0f) * 0.01f};
    }
}