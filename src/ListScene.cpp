#include "ListScene.hpp"
#include "file.hpp"

using namespace rl;

void ListScene::initialise(SDL_Renderer *renderer)
{
    file::loading.wait();
    printf("%lu\n", file::charts.size());
}

void ListScene::draw(SDL_Renderer *renderer)
{
    //
}

void ListScene::release()
{
    //
}