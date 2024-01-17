#pragma once

#include <vector>
#include <string>

namespace rl {
class Sound {
public:
  Sound(const std::string &filename);
  ~Sound();

  void Play();

  std::vector<float> GetSamples(size_t size);

private:
  std::vector<float> m_Buffer;
  size_t m_BufferPos;
};
} // namespace rl