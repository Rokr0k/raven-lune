#include "App.hpp"
#include "TitleScene.hpp"

int main(int argc, char **argv)
{
    rl::App app(new rl::TitleScene(&app));
    return app.loop();
}