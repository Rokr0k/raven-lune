#include "ListScene.hpp"
#include "PlayScene.hpp"
#include "image.hpp"
#include "font.hpp"
#include "file.hpp"
#include <SDL_image.h>

using namespace rl;

void ListScene::initialise()
{
    int w, h;
    loaded = file::loading.wait_for(std::chrono::nanoseconds(1)) == std::future_status::ready;
    selected = false;
    loading = font::renderText(app->renderer, "Loading...");
    SDL_QueryTexture(loading, NULL, NULL, &w, &h);
    loadingRect = {0, 0, (float)w / h * 40, 40};
    if (loaded)
    {
        onload();
    }
}

void ListScene::draw()
{
    if (!loaded)
    {
        loaded = file::loading.wait_for(std::chrono::nanoseconds(1)) == std::future_status::ready;
        SDL_RenderCopyF(app->renderer, loading, NULL, &loadingRect);
        if (loaded)
        {
            onload();
        }
    }
    else if (!selected)
    {
        if (index < infos.size())
        {
            SDL_RenderCopyF(app->renderer, infos[index].genre, NULL, &infos[index].genreRect);
            SDL_RenderCopyF(app->renderer, infos[index].title, NULL, &infos[index].titleRect);
            SDL_RenderCopyF(app->renderer, infos[index].subtitle, NULL, &infos[index].subtitleRect);
            SDL_RenderCopyF(app->renderer, infos[index].artist, NULL, &infos[index].artistRect);
            SDL_RenderCopyF(app->renderer, infos[index].level, NULL, &infos[index].levelRect);
            SDL_RenderCopyF(app->renderer, infos[index].banner, NULL, &infos[index].bannerRect);
        }
    }
    else
    {
        SDL_RenderCopyF(app->renderer, infos[index].stagefile, NULL, &infos[index].stagefileRect);
        if (timer < SDL_GetTicks())
        {
            app->changeScene(new PlayScene(app, chart, autoSelected));
        }
    }
}

void ListScene::onkeydown(SDL_KeyboardEvent key)
{
    switch (key.keysym.sym)
    {
    case SDLK_UP:
        if (loaded && !selected)
        {
            index = (index + infos.size() - 1) % infos.size();
        }
        break;
    case SDLK_DOWN:
        if (loaded && !selected)
        {
            index = (index + 1) % infos.size();
        }
        break;
    case SDLK_RETURN:
        if (loaded && !selected && index < infos.size())
        {
            selected = true;
            autoSelected = false;
            timer = SDL_GetTicks() + 2000;
            chart = file::charts[index];
            file::charts.erase(file::charts.begin() + index);
        }
        break;
    case SDLK_SPACE:
        if (loaded && !selected && index < infos.size())
        {
            selected = true;
            autoSelected = true;
            timer = SDL_GetTicks() + 2000;
            chart = file::charts[index];
            file::charts.erase(file::charts.begin() + index);
        }
        break;
    }
}

void ListScene::release()
{
    SDL_DestroyTexture(loading);
    for (const info_t &info : infos)
    {
        SDL_DestroyTexture(info.genre);
        SDL_DestroyTexture(info.title);
        SDL_DestroyTexture(info.subtitle);
        SDL_DestroyTexture(info.artist);
        SDL_DestroyTexture(info.level);
        SDL_DestroyTexture(info.banner);
        SDL_DestroyTexture(info.stagefile);
    }
}

void ListScene::onload()
{
    index = 0;
    for (const bms::Chart *chart : file::charts)
    {
        info_t info = {};
        int w, h;
        info.genre = font::renderText(app->renderer, chart->genre);
        SDL_QueryTexture(info.genre, NULL, NULL, &w, &h);
        info.genreRect = {20, 20, std::min((float)w / h * 40, 600.0f), 40};
        info.title = font::renderText(app->renderer, chart->title);
        SDL_QueryTexture(info.title, NULL, NULL, &w, &h);
        info.titleRect = {20, 60, std::min((float)w / h * 60, 600.0f), 60};
        info.subtitle = font::renderText(app->renderer, chart->subtitle);
        SDL_QueryTexture(info.subtitle, NULL, NULL, &w, &h);
        info.subtitleRect = {20, 120, std::min((float)w / h * 40, 600.0f), 40};
        info.artist = font::renderText(app->renderer, chart->artist);
        SDL_QueryTexture(info.artist, NULL, NULL, &w, &h);
        info.artistRect = {20, 160, std::min((float)w / h * 40, 600.0f), 40};
        info.level = font::renderText(app->renderer, std::to_string(chart->playLevel));
        SDL_QueryTexture(info.level, NULL, NULL, &w, &h);
        info.levelRect = {20, 200, std::min((float)w / h * 60, 600.0f), 60};
        info.banner = image::loadImage(app->renderer, chart->banner.c_str());
        info.bannerRect = {0, 480 - 1024.0f / 6, 640, 1024.0f / 6};
        info.stagefile = image::loadImage(app->renderer, chart->stagefile.c_str());
        info.stagefileRect = {0, 0, 640, 480};
        switch (chart->difficulty)
        {
        case 0:
            SDL_SetTextureColorMod(info.level, 0x80, 0xff, 0x80);
            break;
        case 1:
            SDL_SetTextureColorMod(info.level, 0x80, 0x80, 0xff);
            break;
        case 2:
            SDL_SetTextureColorMod(info.level, 0xff, 0xff, 0x80);
            break;
        case 3:
            SDL_SetTextureColorMod(info.level, 0xff, 0x80, 0x80);
            break;
        case 4:
            SDL_SetTextureColorMod(info.level, 0xff, 0x80, 0xff);
            break;
        case 5:
            SDL_SetTextureColorMod(info.level, 0xff, 0x00, 0x00);
            break;
        }
        infos.push_back(info);
    }
}