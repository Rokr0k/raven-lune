#include "scenes/title.hpp"
#include <SDL_main.h>
#include <rl/app.hpp>

int main(int argc, char **argv) {
  rl::App *app = new rl::App{};

  app->ChangeScene<rl::TitleScene>();

  app->Run();

  delete app;

  return 0;
}