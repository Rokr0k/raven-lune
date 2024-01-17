#pragma once

#include <SDL_audio.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <string>

namespace rl {
class Scene;

class App {
public:
  App();
  ~App();

  void Run();

  void Quit();

  template <typename _S, typename... Args> void ChangeScene(Args... args) {
    m_NextScene =
        std::unique_ptr<Scene>{dynamic_cast<Scene *>(new _S{this, args...})};
  }

  SDL_Window *GetWindow() const;
  SDL_Renderer *GetRenderer() const;

  const std::string &GetBasePath() const;
  const std::string &GetPrefPath() const;

private:
  std::string m_BasePath;
  std::string m_PrefPath;

  SDL_Window *m_Window;
  SDL_Renderer *m_Renderer;
  SDL_AudioDeviceID m_AudioDevice;

  std::unique_ptr<Scene> m_Scene;
  std::unique_ptr<Scene> m_NextScene;
};
} // namespace rl