#pragma once

#include <rl/font.hpp>
#include <rl/scene.hpp>

namespace rl {
class TitleScene final : public Scene {
public:
  TitleScene(App *app);
  ~TitleScene() final;

  void Iterate() final;
  void Event(const SDL_Event &event) final;

private:
  std::unique_ptr<Font> m_Font;
  SDL_Texture *m_TitleTexture;
  SDL_Texture *m_InstructionTexture;
};
} // namespace rl