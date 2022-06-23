#include "TitleScene.hpp"
#include "ListScene.hpp"
#include "font.hpp"
#include "file.hpp"

using namespace rl;

void TitleScene::initialise()
{
    int w, h;
    title = font::renderText(app->renderer, "Raven Lune");
    SDL_QueryTexture(title, NULL, NULL, &w, &h);
    titleRect = {320 - (float)w / h * 40, 160, (float)w / h * 80, 80};
    instruction = font::renderText(app->renderer, "Press Space to Start");
    SDL_QueryTexture(instruction, NULL, NULL, &w, &h);
    instructionRect = {320 - (float)w / h * 20, 340, (float)w / h * 40, 40};
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
        app->changeScene(new ListScene(app));
        break;
    }
}

void TitleScene::release()
{
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(instruction);
}