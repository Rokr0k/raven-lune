#pragma once

#include <memory>
#include <rl/font.hpp>
#include <rl/scene.hpp>

namespace rl {
class ListScene final : public Scene {
public:
  ListScene(App *app);
  ~ListScene() final;

  void Iterate() final;
  void Event(const SDL_Event &event) final;

private:
  std::unique_ptr<Font> m_Font;
};
} // namespace rl
