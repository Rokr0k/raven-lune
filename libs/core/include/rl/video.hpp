#pragma once

#include <SDL_render.h>
#include <SDL_thread.h>
#include <memory>
#include <string>

struct AVFrame;

namespace rl {
class Video {
public:
  Video(SDL_Renderer *renderer, const std::string &filename);
  ~Video();

  void Wait();

  SDL_Texture *GetFrame(uint64_t timestamp);

private:
  static int Decode(void *userdata);

  std::unique_ptr<AVFrame, void (*)(AVFrame *)> m_Frame;
  SDL_Texture *m_Texture;

  SDL_Thread *m_DecodeThread;
  SDL_mutex *m_Mutex;
  SDL_cond *m_Cond;
  bool m_Abort;
};
} // namespace rl