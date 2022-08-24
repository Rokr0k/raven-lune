#pragma once

#include <queue>
#include <string>
#include <SDL.h>
#include <vlc/vlc.h>

namespace rl
{
    class Video
    {
    private:
        libvlc_instance_t *instance;
        libvlc_media_t *media;
        libvlc_media_player_t *player;
        SDL_Surface *surface;
        struct Picture
        {
            SDL_Surface *surface;
            float time;
            bool operator<(const Picture &r) const
            {
                return time < r.time;
            }
        };
        std::queue<Picture> pictQueue;
        int count;
        bool first;
        Uint32 start;
        float spf;

        static void *lock(void *data, void **p_pixels);
        static void unlock(void *data, void *id, void *const *p_pixels);
        static void display(void *data, void *id);

    public:
        Video(const std::string &file);
        SDL_Surface *operator()(float time);
        ~Video();
    };
}