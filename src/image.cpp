#include "image.hpp"
#include "file.hpp"
#include <SDL_image.h>

using namespace rl;

void image::initialise()
{
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

SDL_Surface *image::loadImage(const std::string &file)
{
    for (const std::string &i : file::getAltFiles(file))
    {
        SDL_Surface *img = IMG_Load(i.c_str());
        if (img)
        {
            return img;
        }
    }
    return NULL;
}

SDL_Texture *image::loadImage(SDL_Renderer *renderer, const std::string &file)
{
    for (const std::string &i : file::getAltFiles(file))
    {
        SDL_Texture *img = IMG_LoadTexture(renderer, i.c_str());
        if (img)
        {
            return img;
        }
    }
    return NULL;
}

void image::release()
{
    IMG_Quit();
}