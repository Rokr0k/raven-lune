#include "bga.hpp"
#include <map>
#include "image.hpp"
#include "video.hpp"

using namespace rl;

static std::map<int, std::pair<SDL_Surface *, bool>> surfaces;

static bool initialised = false;

void bga::load(int key, const std::string &file)
{
    if (!initialised)
    {
        video::initialise();
        initialised = true;
    }
    SDL_Surface *img = image::loadImage(file);
    if (img)
    {
        SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 0x00, 0x00, 0x00));
        surfaces[key] = std::make_pair(img, false);
    }
    else
    {
        video::load(key, file);
    }
}

void bga::start(int key)
{
    if (surfaces.find(key) == surfaces.end())
    {
        video::play(key);
    }
}

SDL_Surface *bga::get(int key)
{
    if (surfaces.find(key) != surfaces.end())
    {
        return surfaces[key].first;
    }
    else
    {
        return video::get(key);
    }
}

void bga::retrn(int key)
{
    if (surfaces.find(key) == surfaces.end())
    {
        video::retrn(key);
    }
}

void bga::release()
{
    for (const std::pair<int, std::pair<SDL_Surface *, bool>> &a : surfaces)
    {
        if (!a.second.second)
        {
            SDL_FreeSurface(a.second.first);
        }
    }
    surfaces.clear();
    if (initialised)
    {
        video::release();
        initialised = false;
    }
}