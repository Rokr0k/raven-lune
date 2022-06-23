#include "audio.hpp"
#include <SDL_mixer.h>

using namespace rl;

static Mix_Chunk *audios[2000];
static Mix_Music *music;

static std::string possibleFormats[] = {".wav", ".ogg", ".flac", ".mp3"};

void audio::initialise()
{
    Mix_Init(MIX_INIT_OGG | MIX_INIT_OPUS | MIX_INIT_FLAC | MIX_INIT_MP3);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
    Mix_AllocateChannels(2000);
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
    if (Mix_Playing(index))
    {
        Mix_HaltChannel(index);
    }
    Mix_PlayChannel(index, audios[index], 0);
}

bool audio::isPlayingAudio()
{
    for (int i = 0; i < 2000; i++)
    {
        if (Mix_Playing(i))
        {
            return true;
        }
    }
    return false;
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
}