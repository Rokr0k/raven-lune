#include <bms/objects.hpp>
#include <cmath>

namespace rl::bms {
Object::Object(beat_t beat, time_t time) : m_Beat{beat}, m_Time{time} {}

Object::~Object() {}

beat_t Object::GetBeat() const { return m_Beat; }

time_t Object::GetTime() const { return m_Time; }

BPM::BPM(beat_t beat, time_t time, bpm_t bpm, bool inclusive)
    : Object{beat, time}, m_BPM{bpm}, m_Inclusive{inclusive} {}

BPM::~BPM() {}

bpm_t BPM::GetBPM() const { return m_BPM; }

bool BPM::GetInclusive() const { return m_Inclusive; }

beat_t BPM::GetBeatFromTime(time_t time) const {
  return GetBeat() + (time - GetTime()) * GetBPM() / 60000;
}

time_t BPM::GetTimeFromBeat(beat_t beat) const {
  return GetTime() +
         (beat - GetBeat()) * 60000 / (GetBPM() != 0 ? GetBPM() : 1);
}

KeyObject::KeyObject(beat_t beat, time_t time, key_t key)
    : Object{beat, time} {}

KeyObject::~KeyObject() {}

int KeyObject::GetKey() const { return m_Key; }

BGM::BGM(beat_t beat, time_t time, key_t key) : KeyObject{beat, time, key} {}

BGM::~BGM() {}

BMP::BMP(beat_t beat, time_t time, key_t key, BMP::Layer layer)
    : KeyObject{beat, time, key}, m_Layer{layer} {}

BMP::~BMP() {}

BMP::Layer BMP::GetLayer() const { return m_Layer; }

LineObject::LineObject(beat_t beat, time_t time, key_t key, Player player,
                       line_t line)
    : KeyObject{beat, time, key}, m_Player{player}, m_Line{line} {}

LineObject::~LineObject() {}

Player LineObject::GetPlayer() const { return m_Player; }

int LineObject::GetLine() const { return m_Line; }

Note::Note(beat_t beat, time_t time, key_t key, Player player, line_t line)
    : LineObject{beat, time, key, player, line}, m_End{nullptr} {}

Note::Note(beat_t beat, time_t time, key_t key, Player player, line_t line,
           beat_t endBeat, time_t endTime)
    : LineObject{beat, time, key, player, line},
      m_End{new Object{endBeat, endTime}} {}

Note::~Note() {}

const std::unique_ptr<Object> &Note::GetEnd() const { return m_End; }

Invisible::Invisible(beat_t beat, time_t time, key_t key, Player player,
                     line_t line)
    : LineObject{beat, time, key, player, line} {}

Invisible::~Invisible() {}

Bomb::Bomb(beat_t beat, time_t time, key_t key, Player player, line_t line)
    : LineObject{beat, time, key, player, line} {}

Bomb::~Bomb() {}

void Signatures::Set(measure_t measure, signature_t signature) {
  m_Signatures.insert_or_assign(measure, signature);
}

signature_t Signatures::Get(measure_t measure) const {
  auto it = m_Signatures.find(measure);
  return it != m_Signatures.end() ? it->second : 1;
}

using ratio_t = float;

beat_t Signatures::GetBeatFromPos(pos_t pos) const {
  measure_t measure = std::floor(pos);
  ratio_t ratio = pos - measure;

  beat_t beat = Get(measure) * ratio;

  for (measure_t m = 0; m < measure; m++) {
    beat += Get(m);
  }

  for (measure_t m = -1; m >= measure; m--) {
    beat -= Get(m);
  }

  return beat * 4;
}

pos_t Signatures::GetPosFromBeat(beat_t beat) const {
  beat /= 4;

  measure_t measure = 0;

  while (beat > 0) {
    beat -= Get(measure++);
  }

  while (beat < 0) {
    beat += Get(--measure);
  }

  return measure + beat / Get(measure);
}
} // namespace rl::bms