#include "app.hpp"
#include "font.hpp"
#include "scene.hpp"
#include <SDL.h>


namespace rl {
App::App()
    : m_Window{nullptr}, m_Renderer{nullptr}, m_AudioDevice{0},
      m_Scene{nullptr} {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  char *basePath = SDL_GetBasePath();
  m_BasePath.assign(basePath);
  SDL_free(basePath);

  char *prefPath = SDL_GetPrefPath("Rokr0k", "RavenLune");
  m_PrefPath.assign(prefPath);
  SDL_free(prefPath);

  m_Window = SDL_CreateWindow("Raven Lune", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 640, 480, 0);

  m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

  SDL_AudioSpec audioSpec;
  audioSpec.freq = 48000;
  audioSpec.channels = 2;
  audioSpec.samples = 128;
  audioSpec.format = AUDIO_F32;
  audioSpec.callback = [](void *userdata, Uint8 *stream, int length) {
    App *app = reinterpret_cast<App *>(userdata);
    app->m_Scene->Audio(reinterpret_cast<float *>(stream),
                        length / sizeof(float));
  };
  audioSpec.userdata = this;
  m_AudioDevice =
      SDL_OpenAudioDevice(nullptr, SDL_FALSE, &audioSpec, nullptr, 0);

  Font::Init();

  m_Scene = std::unique_ptr<Scene>{new Scene{this}};
}

App::~App() {
  m_Scene.reset();

  SDL_DestroyRenderer(m_Renderer);
  SDL_DestroyWindow(m_Window);

  SDL_CloseAudioDevice(m_AudioDevice);

  Font::Quit();

  SDL_Quit();
}

void App::Run() {
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        goto quit;
      else
        m_Scene->Event(event);
    }

    m_Scene->Iterate();
  }
quit:;
}

void App::Quit() {
  SDL_QuitEvent event;
  event.type = SDL_QUIT;
  event.timestamp = SDL_GetTicks();
  SDL_PushEvent(reinterpret_cast<SDL_Event *>(&event));
}

SDL_Window *App::GetWindow() const { return m_Window; }

SDL_Renderer *App::GetRenderer() const { return m_Renderer; }

const std::string &App::GetBasePath() const { return m_BasePath; }

const std::string &App::GetPrefPath() const { return m_PrefPath; }
} // namespace rl