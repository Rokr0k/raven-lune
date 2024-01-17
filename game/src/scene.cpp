#include "scene.hpp"
#include <algorithm>

namespace rl {
Scene::Scene(App *app) : m_App{app} {}

Scene::~Scene() {}

void Scene::Iterate() {
  SDL_SetRenderDrawColor(GetApp()->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(GetApp()->GetRenderer());

  SDL_RenderPresent(GetApp()->GetRenderer());
}

void Scene::Event(const SDL_Event &event) {}

void Scene::Audio(float *stream, int samples) {
  std::fill_n(stream, samples, 0);
}

App *Scene::GetApp() { return m_App; }
} // namespace rl