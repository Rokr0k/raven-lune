#include "video.hpp"
#include <fstream>

using namespace rl;

static std::string possibleFormats[] = {".mpg", ".mpeg", ".mp4", ".webm", ".avi", ".wmv"};

void *Video::lock(void *data, void **p_pixels)
{
    Video *video = (Video *)data;
    SDL_LockSurface(video->surface);
    *p_pixels = video->surface->pixels;
    return NULL;
}

void Video::unlock(void *data, void *id, void *const *p_pixels)
{
    Video *video = (Video *)data;
    SDL_UnlockSurface(video->surface);
}

void Video::display(void *data, void *id)
{
    Video *video = (Video *)data;
    if (video->start == -1)
    {
        video->start = SDL_GetTicks();
    }
    video->pictQueue.push({SDL_DuplicateSurface(video->surface), (SDL_GetTicks() - video->start) * 0.001f});
}

Video::Video(const std::string &file)
{
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 256, 256, 16, 0x001f, 0x07e0, 0xf800, 0);
    instance = libvlc_new(0, NULL);
    for (int i = 0; i < 6; i++)
    {
        std::string path = file.substr(0, file.find_last_of('.')) + possibleFormats[i];
        if (FILE *f = fopen(path.c_str(), "r"))
        {
            fclose(f);
            media = libvlc_media_new_path(instance, path.c_str());
            if (media)
            {
                break;
            }
        }
    }
    player = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
    libvlc_video_set_format(player, "RV16", 256, 256, 512);
    libvlc_video_set_callbacks(player, lock, unlock, display, this);
    libvlc_media_player_play(player);
    start = -1;
}

SDL_Surface *Video::operator()(float time)
{
    while (!pictQueue.empty())
    {
        Picture &picture = pictQueue.front();
        if (picture.time < time)
        {
            SDL_FreeSurface(picture.surface);
            pictQueue.pop();
        }
        else
        {
            return picture.surface;
        }
    }
    return NULL;
}

Video::~Video()
{
    if (surface)
    {
        SDL_FreeSurface(surface);
    }
    while (!pictQueue.empty())
    {
        SDL_FreeSurface(pictQueue.front().surface);
        pictQueue.pop();
    }
    libvlc_media_player_stop(player);
    libvlc_media_player_release(player);
    libvlc_release(instance);
}