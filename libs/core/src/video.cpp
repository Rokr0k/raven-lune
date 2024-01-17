#include <SDL_timer.h>
#include <rl/video.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

struct DecodeArg {
  rl::Video *video;
  std::unique_ptr<std::string> filename;
};

namespace rl {
Video::Video(SDL_Renderer *renderer, const std::string &filename)
    : m_Frame{nullptr,
              [](AVFrame *frame) {
                av_freep(&frame->data[0]);
                av_frame_free(&frame);
              }},
      m_Texture{nullptr}, m_DecodeThread{nullptr}, m_Mutex{nullptr},
      m_Cond{nullptr}, m_Abort{false} {
  DecodeArg *arg = new DecodeArg{this, std::make_unique<std::string>(filename)};

  m_Mutex = SDL_CreateMutex();
  m_Cond = SDL_CreateCond();
  m_DecodeThread = SDL_CreateThread(Video::Decode, "Decode Video", arg);

  m_Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12,
                                SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
}

Video::~Video() {
  m_Frame.reset();
  m_Abort = true;
  SDL_CondSignal(m_Cond);
  SDL_WaitThread(m_DecodeThread, nullptr);

  SDL_DestroyMutex(m_Mutex);
  SDL_DestroyCond(m_Cond);
  SDL_DestroyTexture(m_Texture);
}

void Video::Wait() {
  if (m_DecodeThread != nullptr) {
    SDL_WaitThread(m_DecodeThread, nullptr);
    m_DecodeThread = nullptr;
  }
}

SDL_Texture *Video::GetFrame(uint64_t timestamp) {
  SDL_LockMutex(m_Mutex);

  if (m_Frame != nullptr && m_Frame->best_effort_timestamp < timestamp) {
    SDL_UpdateYUVTexture(m_Texture, nullptr, m_Frame->data[0],
                         m_Frame->linesize[0], m_Frame->data[1],
                         m_Frame->linesize[1], m_Frame->data[2],
                         m_Frame->linesize[2]);

    SDL_CondSignal(m_Cond);

    m_Frame.reset();
  }

  SDL_UnlockMutex(m_Mutex);

  return m_Texture;
}

int Video::Decode(void *userdata) {
  DecodeArg *arg = reinterpret_cast<DecodeArg *>(userdata);

  AVFormatContext *formatCtx = nullptr;
  avformat_open_input(&formatCtx, arg->filename->c_str(), nullptr, nullptr);
  avformat_find_stream_info(formatCtx, nullptr);

  int streamIdx =
      av_find_best_stream(formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
  const AVStream *stream = formatCtx->streams[streamIdx];

  const AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
  AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
  avcodec_parameters_to_context(codecCtx, stream->codecpar);
  avcodec_open2(codecCtx, codec, nullptr);

  SwsContext *swsCtx = sws_getContext(
      codecCtx->width, codecCtx->height, codecCtx->pix_fmt, 1920, 1080,
      AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);

  AVPacket *packet = av_packet_alloc();
  AVFrame *frame = av_frame_alloc();

  while (!arg->video->m_Abort && av_read_frame(formatCtx, packet) == 0) {
    if (packet->stream_index == stream->index) {
      int ret = avcodec_send_packet(codecCtx, packet);
      if (ret != 0)
        continue;

      while (true) {
        ret = avcodec_receive_frame(codecCtx, frame);
        if (ret != 0)
          break;

        AVFrame *outFrame = av_frame_alloc();

        av_image_alloc(outFrame->data, outFrame->linesize, 1920, 1080,
                       AV_PIX_FMT_YUV420P, 32);

        sws_scale(swsCtx, reinterpret_cast<uint8_t const *const *>(frame->data),
                  frame->linesize, 0, codecCtx->height, outFrame->data,
                  outFrame->linesize);

        av_frame_copy_props(outFrame, frame);

        SDL_LockMutex(arg->video->m_Mutex);

        if (arg->video->m_Frame != nullptr) {
          SDL_CondWait(arg->video->m_Cond, arg->video->m_Mutex);
        }

        arg->video->m_Frame = std::unique_ptr<AVFrame, void (*)(AVFrame *)>{
            outFrame, [](AVFrame *frame) {
              av_freep(&frame->data[0]);
              av_frame_free(&frame);
            }};

        SDL_UnlockMutex(arg->video->m_Mutex);
      }
    }

    av_packet_unref(packet);
  }

  avcodec_free_context(&codecCtx);
  avformat_close_input(&formatCtx);
  sws_freeContext(swsCtx);
  av_packet_free(&packet);
  av_frame_free(&frame);

  delete arg;

  return 0;
}
} // namespace rl