#include "title.hpp"
#include <SDL.h>

namespace rl {
TitleScene::TitleScene(App *app)
    : Scene{app}, m_Font{nullptr}, m_TitleTexture{nullptr} {
  m_Font = std::make_unique<Font>(GetApp()->GetBasePath() +
                                  "res/fonts/NotoSansCJK-Regular.ttc");

  m_TitleTexture =
      m_Font->RenderText(GetApp()->GetRenderer(), "Raven Lune", 100);

  m_InstructionTexture =
      m_Font->RenderText(GetApp()->GetRenderer(), "Press space to start", 40);
}

TitleScene::~TitleScene() {
  SDL_DestroyTexture(m_TitleTexture);
  SDL_DestroyTexture(m_InstructionTexture);
}

void TitleScene::Iterate() {
  SDL_SetRenderDrawColor(GetApp()->GetRenderer(), 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(GetApp()->GetRenderer());

  int w, h;
  SDL_GetRendererOutputSize(GetApp()->GetRenderer(), &w, &h);

  SDL_Rect titleRect;
  SDL_QueryTexture(m_TitleTexture, nullptr, nullptr, &titleRect.w,
                   &titleRect.h);
  titleRect.x = w / 2 - titleRect.w / 2;
  titleRect.y = h / 2 - titleRect.h;
  SDL_RenderCopy(GetApp()->GetRenderer(), m_TitleTexture, nullptr, &titleRect);

  SDL_Rect instructionRect;
  SDL_QueryTexture(m_InstructionTexture, nullptr, nullptr, &instructionRect.w,
                   &instructionRect.h);
  instructionRect.x = w / 2 - instructionRect.w / 2;
  instructionRect.y = h * 2 / 3 - instructionRect.h / 2;
  SDL_RenderCopy(GetApp()->GetRenderer(), m_InstructionTexture, nullptr,
                 &instructionRect);

  SDL_RenderPresent(GetApp()->GetRenderer());
}

void TitleScene::Event(const SDL_Event &event) {}
} // namespace rl