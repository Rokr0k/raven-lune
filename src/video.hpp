#pragma once

#include <queue>
#include <string>
#include <SDL.h>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

namespace rl
{
    class Video
    {
    private:
        AVFormatContext *formatCtx;
        int videoStream;
        AVCodecContext *codecCtx;
        AVFrame *frame;
        AVFrame *frameRGB;
        AVPacket *packet;
        SwsContext *swsCtx;
        SDL_mutex *mutex;
        struct Picture
        {
            SDL_Surface *surface;
            float time;
            bool operator<(const Picture &r) const
            {
                return time < r.time;
            }
        };
        std::queue<Picture> pictQueue;

        static int decode(void *data);

    public:
        Video(const std::string &file);
        SDL_Surface *operator()(float time);
        ~Video();
    };
}