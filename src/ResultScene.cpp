#include "ResultScene.hpp"
#include "ListScene.hpp"
#include "keys.hpp"
#include "font.hpp"

using namespace rl;

void ResultScene::initialise()
{
    int w, h;
    cleared = font::renderText(app->renderer, result.cleared ? "Stage Cleared" : "Stage Failed");
    SDL_QueryTexture(cleared, NULL, NULL, &w, &h);
    clearedRect = {320 - (float)w / h * 25, 30, (float)w / h * 50, 50};
    maxCombo = font::renderText(app->renderer, "Max Combo: " + std::to_string(result.maxCombo));
    SDL_QueryTexture(maxCombo, NULL, NULL, &w, &h);
    maxComboRect = {50, 100, (float)w / h * 30, 30};
    std::string judges[5] = {"GREAT", "GREAT", "GOOD", "BAD", "POOR"};
    SDL_Color judgeColors[5] = {{0xcc, 0xcc, 0xcc}, {0xff, 0xd7, 0x00}, {0xad, 0xff, 0x2f}, {0xee, 0x82, 0xee}, {0xdc, 0x14, 0x3c}};
    for (int i = 0; i < 5; i++)
    {
        judge[i] = font::renderText(app->renderer, judges[i] + ": " + std::to_string(result.judgeCount[i]));
        SDL_QueryTexture(judge[i], NULL, NULL, &w, &h);
        judgeRect[i] = {50, 140.0f + 30 * i, (float)w / h * 30, 30};
        SDL_SetTextureColorMod(judge[i], judgeColors[i].r, judgeColors[i].g, judgeColors[i].b);
    }
    std::string ranks[9] = {"F", "E", "D", "C", "B", "A", "AA", "AAA", "S"};
    if (result.noteCount > 0)
    {
        rank = font::renderText(app->renderer, ranks[(result.judgeCount[0] * 2 + result.judgeCount[1]) * 9 / (result.noteCount * 2)]);
    }
    else
    {
        rank = font::renderText(app->renderer, "?");
    }
    SDL_QueryTexture(rank, NULL, NULL, &w, &h);
    rankRect = {590 - (float)w / h * 160, 100, (float)w / h * 160, 160};
}

void ResultScene::draw()
{
    SDL_RenderCopyF(app->renderer, cleared, NULL, &clearedRect);
    SDL_RenderCopyF(app->renderer, maxCombo, NULL, &maxComboRect);
    for (int i = 0; i < 5; i++)
    {
        SDL_RenderCopyF(app->renderer, judge[i], NULL, &judgeRect[i]);
    }
    SDL_RenderCopyF(app->renderer, rank, NULL, &rankRect);
}

void ResultScene::release()
{
    SDL_DestroyTexture(cleared);
    SDL_DestroyTexture(maxCombo);
    for (int i = 0; i < 5; i++)
    {
        SDL_DestroyTexture(judge[i]);
    }
    SDL_DestroyTexture(rank);
}

void ResultScene::onkeydown(SDL_KeyboardEvent key)
{
    if (key.keysym.sym == SDLK_RETURN)
    {
        app->changeScene(new ListScene(app));
    }
}