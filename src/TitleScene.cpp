#include "TitleScene.hpp"
#include "ListScene.hpp"
#include "font.hpp"
#include "file.hpp"

using namespace rl;

void TitleScene::initialise(App *app)
{
    Scene::initialise(app);
    int w, h;
    font::loadSize(80);
    font::loadSize(40);
    title = font::renderText(app->renderer, "Raven Lune", 80);
    SDL_QueryTexture(title, NULL, NULL, &w, &h);
    titleRect = {320 - (float)w / h * 40, 160, (float)w / h * 80, 80};
    instruction = font::renderText(app->renderer, "Press Space to Start", 40);
    SDL_QueryTexture(instruction, NULL, NULL, &w, &h);
    instructionRect = {320 - (float)w / h * 20, 340, (float)w / h * 40, 40};
    font::unloadSize(80);
    font::unloadSize(40);
}

void TitleScene::draw()
{
    SDL_RenderCopyF(app->renderer, title, NULL, &titleRect);
    SDL_RenderCopyF(app->renderer, instruction, NULL, &instructionRect);
}

void TitleScene::onkeydown(SDL_KeyboardEvent key)
{
    switch (key.keysym.sym)
    {
    case SDLK_SPACE:
        app->changeScene(new ListScene());
        break;
    case SDLK_ESCAPE:
    {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    }
    break;
    }
}

void TitleScene::release()
{
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(instruction);
}