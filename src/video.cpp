#include "video.hpp"
#include <fstream>
#include <map>
#include <vlc/vlc.h>

using namespace rl;

struct context
{
    libvlc_media_player_t *mp;
    SDL_Surface *surface;
    SDL_mutex *mutex;
    int n;
};

static std::map<int, context *> videos;

static std::string possibleFormats[] = {".mpg", ".mp4", ".wmv", ".avi"};

static libvlc_instance_t *libvlc;

static void *lock(void *data, void **pixels)
{
    context *ctx = (context *)data;

    SDL_LockMutex(ctx->mutex);
    SDL_LockSurface(ctx->surface);
    *pixels = ctx->surface->pixels;

    return NULL;
}

static void unlock(void *data, void *id, void *const *pixels)
{
    context *ctx = (context *)data;
    SDL_UnlockSurface(ctx->surface);
    SDL_UnlockMutex(ctx->mutex);
}

static void display(void *data, void *id)
{
    (void)data;
}

void video::initialise()
{
    libvlc = libvlc_new(0, NULL);
}

void video::load(int key, const std::string &file)
{
    for (int i = 0; i < 4; i++)
    {
        std::ifstream test(file.substr(0, file.find_last_of('.')) + possibleFormats[i]);
        if (test)
        {
            libvlc_media_t *m = libvlc_media_new_path(libvlc, (file.substr(0, file.find_last_of('.')) + possibleFormats[i]).c_str());
            context *ctx = new context;
            ctx->surface = SDL_CreateRGBSurface(0, 256, 256, 16, 0x001f, 0x07e0, 0xf800, 0);
            ctx->mutex = SDL_CreateMutex();
            ctx->mp = libvlc_media_player_new_from_media(m);
            libvlc_media_release(m);
            libvlc_video_set_callbacks(ctx->mp, lock, unlock, display, ctx);
            libvlc_video_set_format(ctx->mp, "RV16", 256, 256, 512);
            if (libvlc_media_player_is_seekable(ctx->mp))
            {
                libvlc_media_player_play(videos[key]->mp);
            }
            videos[key] = ctx;
            break;
        }
    }
}

void video::play(int key)
{
    if (videos.find(key) != videos.end())
    {
        if (libvlc_media_player_is_playing(videos[key]->mp))
        {
            libvlc_media_player_set_time(videos[key]->mp, 0);
        }
        else
        {
            libvlc_media_player_play(videos[key]->mp);
        }
    }
}

SDL_Surface *video::get(int key)
{
    if (videos.find(key) != videos.end())
    {
        SDL_LockMutex(videos[key]->mutex);
        return videos[key]->surface;
    }
    return NULL;
}

void video::retrn(int key)
{
    if (videos.find(key) != videos.end())
    {
        SDL_UnlockMutex(videos[key]->mutex);
    }
}

void video::release()
{
    for (const std::pair<int, context *> &a : videos)
    {
        libvlc_media_player_stop(a.second->mp);
        libvlc_media_player_release(a.second->mp);
        SDL_FreeSurface(a.second->surface);
        SDL_DestroyMutex(a.second->mutex);
    }
    libvlc_release(libvlc);
}