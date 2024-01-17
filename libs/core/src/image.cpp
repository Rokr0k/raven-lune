#include <rl/image.hpp>
#include <stb_image.h>

namespace rl::img {
SDL_Surface *LoadImage(const std::string &filename) {
  int x, y, n;
  stbi_uc *data = stbi_load(filename.c_str(), &x, &y, &n, 0);

  SDL_PixelFormatEnum format;
  switch (n) {
  case STBI_rgb:
    format = SDL_PIXELFORMAT_RGB24;
    break;
  case STBI_rgb_alpha:
    format = SDL_PIXELFORMAT_RGBA32;
    break;
  default:
    format = SDL_PIXELFORMAT_UNKNOWN;
    break;
  }

  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, x, y, n * 8, format);

  SDL_Surface *image =
      SDL_CreateRGBSurfaceWithFormatFrom(data, x, y, n * 8, x * n, format);

  SDL_BlitSurface(image, nullptr, surface, nullptr);

  SDL_FreeSurface(image);

  stbi_image_free(data);

  return surface;
}
SDL_Texture *LoadImage(SDL_Renderer *renderer, const std::string &filename) {
  SDL_Surface *surface = LoadImage(filename);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}
} // namespace rl::img