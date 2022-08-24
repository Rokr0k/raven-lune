#include "video.hpp"
#include "file.hpp"
#include <fstream>

using namespace rl;

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
    if (video->first)
    {
        video->spf = -1;
        libvlc_media_track_t **tracks = NULL;
        unsigned int count = libvlc_media_tracks_get(video->media, &tracks);
        for (unsigned int i = 0; i < count; i++)
        {
            if (tracks[i]->i_type == libvlc_track_video && tracks[i]->video->i_frame_rate_num && tracks[i]->video->i_frame_rate_den)
            {
                video->spf = (float)tracks[i]->video->i_frame_rate_den / (float)tracks[i]->video->i_frame_rate_num;
                break;
            }
        }
        libvlc_media_tracks_release(tracks, count);
        video->first = false;
        video->start = SDL_GetTicks();
        video->count = 0;
    }
    if (video->spf < 0)
    {
        video->pictQueue.push({SDL_DuplicateSurface(video->surface), (SDL_GetTicks() - video->start) * 0.001f});
    }
    else
    {
        video->pictQueue.push({SDL_DuplicateSurface(video->surface), video->count++ * video->spf});
    }
}

Video::Video(const std::string &file)
{
    surface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, 256, 256, 16, SDL_PIXELFORMAT_BGR565);
    instance = libvlc_new(0, NULL);
    for (const std::string &i : file::getAltFiles(file))
    {
        media = libvlc_media_new_path(instance, i.c_str());
        if (media)
        {
            break;
        }
    }
    player = libvlc_media_player_new_from_media(media);
    libvlc_video_set_format(player, "RV16", 256, 256, 512);
    libvlc_video_set_callbacks(player, lock, unlock, display, this);
    libvlc_media_player_play(player);
    first = true;
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
    libvlc_media_release(media);
    libvlc_release(instance);
}