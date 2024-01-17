#pragma once

#include "app.hpp"
#include <SDL_events.h>

namespace rl {
class Scene {
public:
  Scene(App *app);
  virtual ~Scene();

  virtual void Iterate();
  virtual void Event(const SDL_Event &event);
  virtual void Audio(float *stream, int samples);

  App *GetApp();

private:
  App *m_App;
};
} // namespace rl