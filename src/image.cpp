#include "image.hpp"
#include <SDL_image.h>

using namespace rl;

static std::string possibleFormats[] = {".bmp", ".png", ".jpg"};

void image::initialise()
{
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

SDL_Surface *image::loadImage(const std::string &file)
{
    for (int i = 0; i < 3; i++)
    {
        SDL_Surface *img = IMG_Load((file.substr(0, file.find_last_of('.')) + possibleFormats[i]).c_str());
        if (img)
        {
            return img;
        }
    }
    return NULL;
}

SDL_Texture *image::loadImage(SDL_Renderer *renderer, const std::string &file)
{
    for (int i = 0; i < 3; i++)
    {
        SDL_Texture *img = IMG_LoadTexture(renderer, (file.substr(0, file.find_last_of('.')) + possibleFormats[i]).c_str());
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