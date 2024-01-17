#pragma once

#include <SDL_render.h>
#include <SDL_surface.h>
#include <string>

namespace rl::img {
SDL_Surface *LoadImage(const std::string &filename);
SDL_Texture *LoadImage(SDL_Renderer *renderer, const std::string &filename);
} // namespace rl::img