#include <algorithm>
#include <rl/scene.hpp>

namespace rl {
Scene::Scene(App *app) : m_App{app} {}

Scene::~Scene() {}

void Scene::Iterate() {
  SDL_Renderer *renderer = GetApp()->GetRenderer();

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}

void Scene::Event(const SDL_Event &event) {}

void Scene::Audio(float *stream, int samples) {
  std::fill_n(stream, samples, 0);
}

App *Scene::GetApp() { return m_App; }
} // namespace rl