#include "TitleScene.hpp"
#include "ListScene.hpp"
#include "font.hpp"

using namespace rl;

void TitleScene::initialise(SDL_Renderer *renderer)
{
    SDL_Surface *tempSurface = font::renderText("Raven Lune");
    title = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    titleRect = SDL_FRect{120, 160, 400, 80};
    tempSurface = font::renderText("Press Space to Start");
    instruction = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    instructionRect = SDL_FRect{120, 340, 400, 40};
}

void TitleScene::draw(SDL_Renderer *renderer)
{
    SDL_RenderCopyF(renderer, title, NULL, &titleRect);
    SDL_RenderCopyF(renderer, instruction, NULL, &instructionRect);
}

void TitleScene::onkeydown(SDL_KeyboardEvent key)
{
    app->changeScene(new ListScene(app));
}

void TitleScene::release()
{
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(instruction);
}