#include "video.hpp"
#include "file.hpp"
#include <fstream>
#include <algorithm>
#include <SDL_thread.h>

using namespace rl;

Video::Video(const std::string &file)
    : formatCtx{nullptr},
      videoStream{-1},
      codecCtx{nullptr},
      frame{nullptr},
      frameRGB{nullptr},
      swsCtx{nullptr},
      mutex{nullptr}
{
    for (const std::string &i : file::getAltFiles(file))
    {
        if (avformat_open_input(&formatCtx, i.c_str(), NULL, NULL) == 0)
        {
            break;
        }
    }
    if (!formatCtx)
        return;

    if (avformat_find_stream_info(formatCtx, NULL) < 0)
        return;
    for (int i = 0; i < formatCtx->nb_streams; i++)
    {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            break;
        }
    }
    if (videoStream < 0)
        return;

    const AVCodec *codec = avcodec_find_decoder(formatCtx->streams[videoStream]->codecpar->codec_id);
    if (!codec)
        return;
    codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx)
        return;
    if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[videoStream]->codecpar) < 0)
        return;
    if (avcodec_open2(codecCtx, codec, NULL) < 0)
        return;

    frame = av_frame_alloc();
    if (!frame)
        return;

    frameRGB = av_frame_alloc();
    if (!frameRGB)
        return;

    packet = av_packet_alloc();
    if (!packet)
        return;

    swsCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt, 256, 256, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    if (!swsCtx)
        return;

    SDL_Thread *thread = SDL_CreateThread(Video::decode, "RL Video Decode", this);
    mutex = SDL_CreateMutex();

    SDL_DetachThread(thread);
}

int Video::decode(void *data)
{
    Video *video = (Video *)data;

    while (av_read_frame(video->formatCtx, video->packet) >= 0)
    {
        if (video->packet->stream_index == video->videoStream)
        {
            avcodec_send_packet(video->codecCtx, video->packet);

            int ret = avcodec_receive_frame(video->codecCtx, video->frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            {
                break;
            }
            else if (ret < 0)
            {
                return -1;
            }

            sws_scale(video->swsCtx, video->frame->data, video->frame->linesize, 0, video->codecCtx->height, video->frameRGB->data, video->frameRGB->linesize);

            SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 256, 256, 24, SDL_PIXELFORMAT_RGB24);
            SDL_LockSurface(surface);
            memcpy(surface->pixels, video->frameRGB->data, 256 * 256 * 3);
            SDL_UnlockSurface(surface);

            SDL_LockMutex(video->mutex);
            video->pictQueue.push(Video::Picture{surface, video->frameRGB->pts * (float)av_q2d(video->codecCtx->time_base)});
            SDL_UnlockMutex(video->mutex);
        }
    }

    return 0;
}

SDL_Surface *Video::operator()(float time)
{
    SDL_LockMutex(mutex);
    while (!pictQueue.empty())
    {
        Picture &picture = pictQueue.front();
        if (picture.time < time)
        {
            SDL_FreeSurface(picture.surface);
            pictQueue.pop();
        }
        else
        {
            SDL_UnlockMutex(mutex);
            return picture.surface;
        }
    }
    SDL_UnlockMutex(mutex);
    return NULL;
}

Video::~Video()
{
    if (formatCtx)
    {
        avformat_close_input(&formatCtx);
    }
    if (codecCtx)
    {
        avcodec_free_context(&codecCtx);
    }
    if (frame)
    {
        av_frame_free(&frame);
    }
    if (frameRGB)
    {
        av_frame_free(&frameRGB);
    }
    if (packet)
    {
        av_packet_free(&packet);
    }
    if (swsCtx)
    {
        sws_freeContext(swsCtx);
    }
    while (!pictQueue.empty())
    {
        SDL_FreeSurface(pictQueue.front().surface);
        pictQueue.pop();
    }
}