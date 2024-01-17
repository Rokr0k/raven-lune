#include <rl/audio.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

namespace rl {
Sound::Sound(const std::string &filename) : m_BufferPos{0} {
  AVFormatContext *formatCtx = nullptr;
  avformat_open_input(&formatCtx, filename.c_str(), nullptr, nullptr);
  avformat_find_stream_info(formatCtx, nullptr);

  int streamIdx =
      av_find_best_stream(formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
  const AVStream *stream = formatCtx->streams[streamIdx];

  const AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
  AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
  avcodec_parameters_to_context(codecCtx, stream->codecpar);
  avcodec_open2(codecCtx, codec, nullptr);

  SwrContext *swrCtx = nullptr;
  AVChannelLayout channelLayout = AV_CHANNEL_LAYOUT_STEREO;
  AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_FLT;
  int out_sample_rate = 48000;
  swr_alloc_set_opts2(&swrCtx, &channelLayout, out_sample_fmt, out_sample_rate,
                      &codecCtx->ch_layout, codecCtx->sample_fmt,
                      codecCtx->sample_rate, 0, nullptr);
  swr_init(swrCtx);

  AVPacket *packet = av_packet_alloc();
  AVFrame *frame = av_frame_alloc();

  int out_nb_channels = channelLayout.nb_channels;
  uint8_t *out_data[1]{nullptr};
  int out_linesize;

  int ret;

  while (av_read_frame(formatCtx, packet) == 0) {
    if (packet->stream_index == streamIdx) {
      ret = avcodec_send_packet(codecCtx, packet);
      if (ret != 0) {
        continue;
      }

      while (true) {
        ret = avcodec_receive_frame(codecCtx, frame);
        if (ret != 0)
          break;

        int out_nb_samples = av_rescale_rnd(
            swr_get_delay(swrCtx, frame->sample_rate) + frame->nb_samples,
            out_sample_rate, frame->sample_rate, AV_ROUND_UP);

        av_samples_alloc(out_data, &out_linesize, out_nb_channels,
                         out_nb_samples, out_sample_fmt, 0);

        int nb_samples = swr_convert(swrCtx, out_data, out_nb_samples,
                                     const_cast<const uint8_t **>(frame->data),
                                     frame->nb_samples);

        int bufsize = av_samples_get_buffer_size(&out_linesize, out_nb_channels,
                                                 nb_samples, out_sample_fmt, 0);

        m_Buffer.insert(m_Buffer.end(), reinterpret_cast<float *>(out_data[0]),
                        reinterpret_cast<float *>(out_data[0] + bufsize));

        av_freep(&out_data[0]);
      }
    }

    av_packet_unref(packet);
  }

  av_packet_free(&packet);
  av_frame_free(&frame);

  swr_free(&swrCtx);

  avcodec_free_context(&codecCtx);

  avformat_close_input(&formatCtx);
}

void Sound::Play() { m_BufferPos = 0; }

std::vector<float> Sound::GetSamples(size_t size) {
  std::vector<float> samples(size);

  std::fill_n(samples.begin(), size, 0);

  if (m_BufferPos < m_Buffer.size()) {
    std::copy_n(m_Buffer.begin() + m_BufferPos,
                std::min(size, m_Buffer.size() - m_BufferPos), samples.begin());
    m_BufferPos = std::min(m_BufferPos + size, m_Buffer.size());
  }

  return samples;
}
} // namespace rl