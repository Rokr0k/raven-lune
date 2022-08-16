#include "bga.hpp"
#include <map>
#include "image.hpp"
#include "video.hpp"

using namespace rl;

struct BGA
{
public:
    union
    {
        SDL_Surface *image;
        Video *video;
    };
    enum class Type
    {
        IMAGE,
        VIDEO
    } type;

    BGA() {}

    BGA(SDL_Surface *image)
    {
        this->image = image;
        type = Type::IMAGE;
    }

    BGA(Video *video)
    {
        this->video = video;
        type = Type::VIDEO;
    }
};

static std::map<int, BGA> textures;

void bga::load(int key, const std::string &file)
{
    SDL_Surface *img = image::loadImage(file);
    if (img)
    {
        SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 0x00, 0x00, 0x00));
        textures[key] = img;
    }
    else
    {
        textures[key] = new Video(file);
    }
}

SDL_Surface *bga::get(int key, float time)
{
    if (textures.find(key) != textures.end())
    {
        switch (textures[key].type)
        {
        case BGA::Type::IMAGE:
            return textures[key].image;
        case BGA::Type::VIDEO:
            return (*textures[key].video)(time);
        }
    }
    return NULL;
}

void bga::release()
{
    for (const std::pair<int, BGA> &a : textures)
    {
        switch (a.second.type)
        {
        case BGA::Type::IMAGE:
            SDL_FreeSurface(a.second.image);
            break;
        case BGA::Type::VIDEO:
            delete a.second.video;
            break;
        }
    }
    textures.clear();
}