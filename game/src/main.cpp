#include "app.hpp"
#include <SDL_main.h>

int main(int argc, char **argv) {
  rl::App *app = new rl::App{};

  app->Run();

  delete app;

  return 0;
}