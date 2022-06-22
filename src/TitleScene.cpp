#include "TitleScene.hpp"
#include "ListScene.hpp"
#include "font.hpp"
#include "file.hpp"

using namespace rl;

void TitleScene::initialise(SDL_Renderer *renderer)
{
    int w, h;
    title = font::renderText(renderer, "Raven Lune");
    SDL_QueryTexture(title, NULL, NULL, &w, &h);
    titleRect = {320 - (float)w / h * 40, 160, (float)w / h * 80, 80};
    instruction = font::renderText(renderer, "Press Space to Start");
    SDL_QueryTexture(instruction, NULL, NULL, &w, &h);
    instructionRect = {320 - (float)w / h * 20, 340, (float)w / h * 40, 40};
}

void TitleScene::draw(SDL_Renderer *renderer)
{
    SDL_RenderCopyF(renderer, title, NULL, &titleRect);
    SDL_RenderCopyF(renderer, instruction, NULL, &instructionRect);
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