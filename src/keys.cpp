#include "keys.hpp"
#include <map>
#include <fstream>

using namespace rl;

static std::map<keys::KeysType, SDL_Keycode> keymap;

void keys::load()
{
    std::ifstream input("res/keys.txt");
    for (int i = 0; i < 27; i++)
    {
        input >> keymap[(keys::KeysType)i];
    }
    input.close();
}

void keys::save()
{
    std::ofstream output("res/keys.txt");
    for (int i = 0; i < 27; i++)
    {
        output << keymap[(keys::KeysType)i] << std::endl;
    }
    output.close();
}

void keys::setKey(keys::KeysType type, SDL_Keycode key)
{
    keymap[type] = key;
}

SDL_Keycode keys::getKey(keys::KeysType type)
{
    if (keymap.find(type) != keymap.end())
    {
        return keymap[type];
    }
    else
    {
        return -1;
    }
}