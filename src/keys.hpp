#pragma once

#include <SDL.h>

namespace rl
{
    namespace keys
    {
        enum KeysType
        {
            S_SU,
            S_SD,
            S_1,
            S_2,
            S_3,
            S_4,
            S_5,
            S_8,
            S_9,
            D1_SU,
            D1_SD,
            D1_1,
            D1_2,
            D1_3,
            D1_4,
            D1_5,
            D1_8,
            D1_9,
            D2_1,
            D2_2,
            D2_3,
            D2_4,
            D2_5,
            D2_8,
            D2_9,
            D2_SU,
            D2_SD,
        };
        void load();
        void save();
        void setKey(KeysType type, SDL_Keycode key);
        SDL_Keycode getKey(KeysType type);
    }
}