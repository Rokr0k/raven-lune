#pragma once

#include <string>

namespace rl
{
    namespace audio
    {
        void initialise();
        void loadAudio(int index, const std::string &file);
        void playAudio(int index);
        void stopAudio(int index);
        void freeMemoryAudio();
        bool isPlayingAudio();
        void releaseAudio();
        void playMusic(const std::string &file);
        void stopMusic();
        void release();
    }
}