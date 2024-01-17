#include "app.hpp"
#include "title.hpp"
#include <SDL_main.h>

int main(int argc, char **argv) {
  rl::App *app = new rl::App{};

  app->ChangeScene<rl::TitleScene>();

  app->Run();

  delete app;

  return 0;
}