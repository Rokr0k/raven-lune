#pragma once

#include "Scene.hpp"
#include <future>
#include <vector>
#include <bmsparser.hpp>

namespace rl
{
    class ListScene : public Scene
    {
    private:
        bool loaded;
        bool selected;
        bool autoSelected;    
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
        size_t index;
        bms::Chart *chart;
        void onload();
        static size_t gindex;

    public:
        ListScene(App *app) : Scene(app) {}
        void initialise();
        void draw();
        void release();
        void onkeydown(SDL_KeyboardEvent event);
    };
}