#include "KeysScene.hpp"
#include "ListScene.hpp"
#include "keys.hpp"
#include "font.hpp"

using namespace rl;

void KeysScene::initialise()
{
    keys::load();
    keyType = 0;
    mode = 0;
    selected = false;
    keyRects[0] = {50, 50, 30, 20};
    keyRects[1] = {50, 70, 30, 20};
    keyRects[2] = {80, 50, 20, 40};
    keyRects[3] = {100, 50, 15, 40};
    keyRects[4] = {115, 50, 20, 40};
    keyRects[5] = {135, 50, 15, 40};
    keyRects[6] = {150, 50, 20, 40};
    keyRects[7] = {170, 50, 15, 40};
    keyRects[8] = {185, 50, 20, 40};

    keyRects[9] = {50, 50, 30, 20};
    keyRects[10] = {50, 70, 30, 20};
    keyRects[11] = {80, 50, 20, 40};
    keyRects[12] = {100, 50, 15, 40};
    keyRects[13] = {115, 50, 20, 40};
    keyRects[14] = {135, 50, 15, 40};
    keyRects[15] = {150, 50, 20, 40};
    keyRects[16] = {170, 50, 15, 40};
    keyRects[17] = {185, 50, 20, 40};
    keyRects[18] = {205, 50, 20, 40};
    keyRects[19] = {225, 50, 15, 40};
    keyRects[20] = {240, 50, 20, 40};
    keyRects[21] = {260, 50, 15, 40};
    keyRects[22] = {275, 50, 20, 40};
    keyRects[23] = {295, 50, 15, 40};
    keyRects[24] = {310, 50, 20, 40};
    keyRects[25] = {330, 50, 30, 20};
    keyRects[26] = {330, 70, 30, 20};

    keyColors[0] = keyColors[1] = keyColors[9] = keyColors[10] = keyColors[25] = keyColors[26] = {0xff, 0x00, 0x00};
    keyColors[2] = keyColors[4] = keyColors[6] = keyColors[8] = keyColors[11] = keyColors[13] = keyColors[15] = keyColors[17] = keyColors[18] = keyColors[20] = keyColors[22] = keyColors[24] = {0xff, 0xff, 0xff};
    keyColors[3] = keyColors[5] = keyColors[7] = keyColors[12] = keyColors[14] = keyColors[16] = keyColors[19] = keyColors[21] = keyColors[23] = {0x00, 0xff, 0xff};

    for (int i = 0; i < 27; i++)
    {
        pressed[i] = false;
    }

    ranges = {{0, 9}, {9, 27}};
}

void KeysScene::draw()
{
    SDL_FRect a;
    for (int i = ranges[mode].first; i < ranges[mode].second; i++)
    {
        SDL_SetRenderDrawColor(app->renderer, keyColors[i].r, keyColors[i].g, keyColors[i].b, 0xff);
        SDL_RenderFillRectF(app->renderer, &keyRects[i]);
        if (pressed[i])
        {
            a = {keyRects[i].x + 2, keyRects[i].y + 2, keyRects[i].w - 4, keyRects[i].h - 4};
            SDL_SetRenderDrawColor(app->renderer, 0x80, 0x80, 0x80, 0xff);
            SDL_RenderDrawRectF(app->renderer, &a);
        }
    }
    if (selected)
    {
        SDL_SetRenderDrawColor(app->renderer, 0xf, 0xff, 0x00, 0xff);
    }
    else
    {
        SDL_SetRenderDrawColor(app->renderer, 0x00, 0x00, 0x00, 0xff);
    }
    a = {keyRects[keyType].x + 5, keyRects[keyType].y + 5, keyRects[keyType].w - 10, keyRects[keyType].h - 10};
    SDL_RenderFillRectF(app->renderer, &a);
}

void KeysScene::release()
{
    keys::save();
}

void KeysScene::onkeydown(SDL_KeyboardEvent key)
{
    if (selected)
    {
        if (key.keysym.sym == SDLK_ESCAPE)
        {
            selected = false;
        }
        else if (key.keysym.sym != SDLK_UP && key.keysym.sym != SDLK_DOWN && key.keysym.sym != SDLK_RETURN && key.keysym.sym != SDLK_BACKSPACE)
        {
            keys::setKey((keys::KeysType)keyType, key.keysym.sym);
            selected = false;
        }
    }
    else
    {
        for (int i = ranges[mode].first; i < ranges[mode].second; i++)
        {
            if (key.keysym.sym == keys::getKey((keys::KeysType)i))
            {
                pressed[i] = true;
                break;
            }
        }
        if (key.keysym.sym == SDLK_RETURN)
        {
            selected = true;
        }
        if (key.keysym.sym == SDLK_ESCAPE)
        {
            app->changeScene(new ListScene(app));
        }
        if (key.keysym.sym == SDLK_UP)
        {
            keyType = (keyType - ranges[mode].first + 1) % (ranges[mode].second - ranges[mode].first) + ranges[mode].first;
        }
        if (key.keysym.sym == SDLK_DOWN)
        {
            keyType = (keyType - ranges[mode].first + ranges[mode].second - ranges[mode].first - 1) % (ranges[mode].second - ranges[mode].first) + ranges[mode].first;
        }
        if (key.keysym.sym == SDLK_BACKSPACE)
        {
            mode = (mode + 1) % ranges.size();
            keyType = ranges[mode].first;
        }
    }
}

void KeysScene::onkeyup(SDL_KeyboardEvent key)
{
    if (!selected)
    {
        for (int i = ranges[mode].first; i < ranges[mode].second; i++)
        {
            if (key.keysym.sym == keys::getKey((keys::KeysType)i))
            {
                pressed[i] = false;
                break;
            }
        }
    }
}