#include "list.hpp"

namespace rl {
ListScene::ListScene(App *app) : Scene{app}, m_Font{nullptr} {
  SDL_Renderer *renderer = GetApp()->GetRenderer();

  m_Font = std::make_unique<Font>(GetApp()->GetBasePath() +
                                  "res/fonts/NotoSansCJK-Regular.ttc");
}

ListScene::~ListScene() {}

void ListScene::Iterate() {
  SDL_Renderer *renderer = GetApp()->GetRenderer();

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}

void ListScene::Event(const SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
      GetApp()->Quit();
    }
  }
}
} // namespace rl
