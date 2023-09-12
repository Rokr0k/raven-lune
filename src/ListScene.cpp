#include "ListScene.hpp"
#include "App.hpp"
#include "TitleScene.hpp"
#include "KeysScene.hpp"
#include "PlayScene.hpp"
#include "image.hpp"
#include "font.hpp"
#include "file.hpp"
#include "keys.hpp"
#include <bmsparser/convert.hpp>

using namespace rl;

std::size_t ListScene::gindex = 0;

void ListScene::initialise(App *app)
{
    Scene::initialise(app);
    int w, h;
    loaded = false;
    selected = false;
    font::loadSize(40);
    loading = font::renderText(app->renderer, "Loading...", 40);
    SDL_QueryTexture(loading, NULL, NULL, &w, &h);
    loadingRect = {0, 0, (float)w / h * 40, 40};
    noCharts = font::renderText(app->renderer, "No charts available.", 40);
    SDL_QueryTexture(noCharts, NULL, NULL, &w, &h);
    noChartsRect = {0, 0, (float)w / h * 40, 40};
    font::unloadSize(40);
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
        if (infos.empty())
        {
            SDL_RenderCopyF(app->renderer, noCharts, NULL, &noChartsRect);
        }
        else if (index < infos.size())
        {
            SDL_RenderCopyF(app->renderer, infos[index].genre, NULL, &infos[index].genreRect);
            SDL_RenderCopyF(app->renderer, infos[index].title, NULL, &infos[index].titleRect);
            SDL_RenderCopyF(app->renderer, infos[index].subtitle, NULL, &infos[index].subtitleRect);
            SDL_RenderCopyF(app->renderer, infos[index].artist, NULL, &infos[index].artistRect);
            SDL_RenderCopyF(app->renderer, infos[index].level, NULL, &infos[index].levelRect);
            SDL_RenderCopyF(app->renderer, infos[index].banner, NULL, &infos[index].bannerRect);
            SDL_RenderCopyF(app->renderer, infos[index].filename, NULL, &infos[index].filenameRect);
        }
    }
    else
    {
        SDL_RenderCopyF(app->renderer, infos[index].stagefile, NULL, &infos[index].stagefileRect);
        if (timer < SDL_GetTicks())
        {
            app->changeScene(new PlayScene(file::charts[index], autoSelected));
        }
    }
}

void ListScene::onkeydown(SDL_KeyboardEvent key)
{
    switch (key.keysym.sym)
    {
    case SDLK_LEFT:
        if (loaded && !selected)
        {
            index = (index + infos.size() - 1) % infos.size();
        }
        break;
    case SDLK_RIGHT:
        if (loaded && !selected)
        {
            index = (index + 1) % infos.size();
        }
        break;
    case SDLK_RETURN:
        if (loaded && !selected && index < infos.size())
        {
            selected = true;
            autoSelected = key.keysym.mod & KMOD_SHIFT;
            timer = SDL_GetTicks() + 2000;
        }
        break;
    case SDLK_r:
        if (loaded && !selected)
        {
            loaded = false;
            file::release();
            file::initialise();
            index = 0;
        }
        break;
    case SDLK_k:
        if (!selected)
        {
            app->changeScene(new KeysScene());
        }
        break;
    case SDLK_ESCAPE:
        app->changeScene(new TitleScene());
        break;
    }
}

void ListScene::release()
{
    gindex = index;
    SDL_DestroyTexture(loading);
    SDL_DestroyTexture(noCharts);
    for (const info_t &info : infos)
    {
        SDL_DestroyTexture(info.genre);
        SDL_DestroyTexture(info.title);
        SDL_DestroyTexture(info.subtitle);
        SDL_DestroyTexture(info.artist);
        SDL_DestroyTexture(info.level);
        SDL_DestroyTexture(info.banner);
        SDL_DestroyTexture(info.stagefile);
        SDL_DestroyTexture(info.filename);
    }
}

void ListScene::onload()
{
    index = gindex;
    font::loadSize(60);
    font::loadSize(40);
    font::loadSize(20);
    for (const bms::Chart *chart : file::charts)
    {
        info_t info = {};
        int w, h;
        info.genre = font::renderText(app->renderer, bms::sjis_to_utf8(chart->genre), 40);
        SDL_QueryTexture(info.genre, NULL, NULL, &w, &h);
        info.genreRect = {20, 20, std::min((float)w / h * 40, 600.0f), 40};
        info.title = font::renderText(app->renderer, bms::sjis_to_utf8(chart->title), 60);
        SDL_QueryTexture(info.title, NULL, NULL, &w, &h);
        info.titleRect = {20, 60, std::min((float)w / h * 60, 600.0f), 60};
        info.subtitle = font::renderText(app->renderer, bms::sjis_to_utf8(chart->subtitle), 40);
        SDL_QueryTexture(info.subtitle, NULL, NULL, &w, &h);
        info.subtitleRect = {20, 120, std::min((float)w / h * 40, 600.0f), 40};
        info.artist = font::renderText(app->renderer, bms::sjis_to_utf8(chart->artist), 40);
        SDL_QueryTexture(info.artist, NULL, NULL, &w, &h);
        info.artistRect = {20, 160, std::min((float)w / h * 40, 600.0f), 40};
        info.level = font::renderText(app->renderer, std::to_string(chart->playLevel), 60);
        SDL_QueryTexture(info.level, NULL, NULL, &w, &h);
        info.levelRect = {20, 200, std::min((float)w / h * 60, 600.0f), 60};
        info.banner = image::loadImage(app->renderer, chart->banner);
        info.bannerRect = {0, 480 - 1024.0f / 6, 640, 1024.0f / 6};
        info.stagefile = image::loadImage(app->renderer, chart->stagefile);
        info.stagefileRect = {0, 0, 640, 480};
        info.filename = font::renderText(app->renderer, chart->filename, 20);
        SDL_QueryTexture(info.filename, NULL, NULL, &w, &h);
        info.filenameRect = {0, 460 - 1024.0f / 6, std::min((float)w / h * 20, 640.0f), 20};
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
    font::unloadSize(60);
    font::unloadSize(40);
    font::unloadSize(20);
}