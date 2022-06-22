#include "App.hpp"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "image.hpp"
#include "font.hpp"
#include "file.hpp"
#include "audio.hpp"

using namespace rl;

App::App(Scene *initialScene)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    image::initialise();
    font::initialise("res/DotGothic16.ttf");
    file::initialise();
    audio::initialise();
    window = SDL_CreateWindow("Raven Lune", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    scene = initialScene;
    if (scene)
    {
        scene->initialise(renderer);
    }
}

App::~App()
{
    if (scene)
    {
        delete scene;
    }
    image::release();
    font::release();
    file::release();
    audio::release();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int App::loop()
{
    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
                if (scene)
                {
                    scene->onkeydown(event.key);
                }
                break;
            case SDL_KEYUP:
                if (scene)
                {
                    scene->onkeyup(event.key);
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        if (scene)
        {
            scene->draw(renderer);
        }
        SDL_RenderPresent(renderer);
    }
    return 0;
}

void App::changeScene(Scene *newScene)
{
    scene->release();
    delete scene;
    scene = newScene;
    scene->initialise(renderer);
}