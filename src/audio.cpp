#include "audio.hpp"
#include <SDL_mixer.h>
#include <map>

using namespace rl;

static Mix_Chunk *audios[2000];
static Mix_Music *music;

static std::map<int, int> channelMap;

static std::string possibleFormats[] = {".wav", ".ogg", ".flac", ".mp3"};

void audio::initialise()
{
    Mix_Init(MIX_INIT_OGG | MIX_INIT_OPUS | MIX_INIT_FLAC | MIX_INIT_MP3);
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
}

void audio::loadAudio(int index, const std::string &file)
{
    if (audios[index])
    {
        Mix_FreeChunk(audios[index]);
        audios[index] = NULL;
    }
    for (int i = 0; i < 4; i++)
    {
        audios[index] = Mix_LoadWAV((file.substr(0, file.find_last_of('.')) + possibleFormats[i]).c_str());
        if (audios[index])
        {
            break;
        }
    }
}

void audio::playAudio(int index)
{
    if (audios[index])
    {
        if (channelMap.find(index) != channelMap.end() && Mix_Playing(channelMap[index]))
        {
            Mix_HaltChannel(channelMap[index]);
        }
        int channel = Mix_PlayChannel(-1, audios[index], 0);
        if (channel == -1)
        {
            int len = Mix_AllocateChannels(-1);
            Mix_AllocateChannels(len + 1);
            channel = Mix_PlayChannel(len, audios[index], 0);
        }
        if (channel >= 0)
        {
            for (std::map<int, int>::iterator i = channelMap.begin(), n = i; i != channelMap.end(); i = n)
            {
                ++n;
                if (i->second == channel)
                {
                    channelMap.erase(i);
                }
            }
            channelMap[index] = channel;
        }
    }
}

void audio::stopAudio(int index)
{
    if (audios[index])
    {
        if (channelMap.find(index) != channelMap.end() && Mix_Playing(channelMap[index]))
        {
            Mix_HaltChannel(channelMap[index]);
            channelMap.erase(index);
        }
    }
}

void audio::freeMemoryAudio()
{
    int len;
    for (len = Mix_AllocateChannels(-1); len > 0 && !Mix_Playing(len - 1); len--)
        ;
    Mix_AllocateChannels(len);
}

bool audio::isPlayingAudio()
{
    return Mix_Playing(-1) > 0;
}

void audio::releaseAudio()
{
    for (int i = 0; i < 2000; i++)
    {
        if (audios[i])
        {
            Mix_FreeChunk(audios[i]);
            audios[i] = NULL;
        }
    }
}

void audio::playMusic(const std::string &file)
{
    if (music)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
    music = Mix_LoadMUS(file.c_str());
    Mix_PlayMusic(music, -1);
}

void audio::stopMusic()
{
    Mix_HaltMusic();
    if (music)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
}

void audio::release()
{
    for (int i = 0; i < 2000; i++)
    {
        if (audios[i])
        {
            Mix_FreeChunk(audios[i]);
            audios[i] = NULL;
        }
    }
    if (music)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
    Mix_CloseAudio();
    Mix_Quit();
    printf("\033[?1049l");
}