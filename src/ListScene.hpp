#pragma once

#include "Scene.hpp"
#include <future>
#include <vector>
#include <bmsparser/bmsparser.hpp>

namespace rl
{
    class ListScene : public Scene
    {
    private:
        bool loaded;
        bool selected;
        Uint32 timer;
        SDL_Texture *loading;
        SDL_FRect loadingRect;
        struct info_t
        {
            SDL_Texture *genre;
            SDL_FRect genreRect;
            SDL_Texture *title;
            SDL_FRect titleRect;
            SDL_Texture *subtitle;
            SDL_FRect subtitleRect;
            SDL_Texture *artist;
            SDL_FRect artistRect;
            SDL_Texture *level;
            SDL_FRect levelRect;
            SDL_Texture *banner;
            SDL_FRect bannerRect;
            SDL_Texture *stagefile;
            SDL_FRect stagefileRect;
        };
        std::vector<info_t> infos;
        std::size_t index;
        bms::Chart *chart;
        void onload(SDL_Renderer *renderer);

    public:
        ListScene(App *app) : Scene(app) {}
        void initialise(SDL_Renderer *renderer);
        void draw(SDL_Renderer *renderer);
        void release();
        void onkeydown(SDL_KeyboardEvent event);
    };
}