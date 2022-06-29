#include "bga.hpp"
#include <map>
#include "image.hpp"

using namespace rl;

static std::map<int, std::pair<SDL_Texture *, bool>> textures;

void bga::load(SDL_Renderer *renderer, int key, const std::string &file)
{
    SDL_Surface *img = image::loadImage(file);
    if (img)
    {
        SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 0x00, 0x00, 0x00));
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, img);
        SDL_FreeSurface(img);
        textures[key] = std::make_pair(tex, false);
    }
}

SDL_Texture *bga::get(int key)
{
    if (textures.find(key) != textures.end())
    {
        return textures[key].first;
    }
    return NULL;
}

void bga::release()
{
    for (const std::pair<int, std::pair<SDL_Texture *, bool>> &a : textures)
    {
        if (!a.second.second)
        {
            SDL_DestroyTexture(a.second.first);
        }
    }
    textures.clear();
}