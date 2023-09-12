#include "App.hpp"
#include "Scene.hpp"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "image.hpp"
#include "font.hpp"
#include "file.hpp"
#include "audio.hpp"
#include "keys.hpp"
#include <GL/gl.h>

using namespace rl;

App::App(Scene *initialScene)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    image::initialise();
    font::initialise("res/fonts/NotoSansCJK-Regular.ttc");
    audio::initialise();
    file::initialise();
    keys::load();
    window = SDL_CreateWindow("Raven Lune", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    gl = SDL_GL_CreateContext(window);
    scene = initialScene;
    if (scene)
    {
        scene->initialise(this);
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
    audio::release();
    file::release();
    SDL_GL_DeleteContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int App::loop()
{
    SDL_Event event;
    bool fstoggle = false;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_F11)
                {
                    fstoggle = !fstoggle;
                    SDL_SetWindowFullscreen(window, fstoggle ? SDL_WINDOW_FULLSCREEN : 0);
                }
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

        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        glViewport(0, 0, w, h);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (scene)
        {
            scene->draw();
        }
        SDL_GL_SwapWindow(window);
    }
    return 0;
}

void App::changeScene(Scene *newScene)
{
    scene->release();
    delete scene;
    scene = newScene;
    scene->initialise(this);
}