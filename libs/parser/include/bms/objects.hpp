#pragma once

#include <map>
#include <memory>

namespace rl::bms {
using beat_t = float;
using time_t = long;
using bpm_t = float;
using key_t = int;
using line_t = int;
using measure_t = int;
using ratio_t = float;
using signature_t = float;
using pos_t = float;

class Object {
public:
  Object(beat_t beat, time_t time);
  virtual ~Object();

  beat_t GetBeat() const;
  time_t GetTime() const;

private:
  beat_t m_Beat;
  time_t m_Time;
};

class BPM : public Object {
public:
  BPM(beat_t beat, time_t time, bpm_t bpm, bool inclusive);
  virtual ~BPM() override;

  bpm_t GetBPM() const;
  bool GetInclusive() const;

  beat_t GetBeatFromTime(time_t time) const;
  time_t GetTimeFromBeat(beat_t beat) const;

private:
  bpm_t m_BPM;
  bool m_Inclusive;
};

class KeyObject : public Object {
public:
  KeyObject(beat_t beat, time_t time, key_t key);
  virtual ~KeyObject() override;

  key_t GetKey() const;

private:
  key_t m_Key;
};

class BGM : public KeyObject {
public:
  BGM(beat_t beat, time_t time, key_t key);
  virtual ~BGM() override;
};

class BMP : public KeyObject {
public:
  enum class Layer {
    Poor = -1,
    Base = 0,
    Up = 1,
  };

  BMP(beat_t beat, time_t time, key_t key, Layer layer);
  virtual ~BMP() override;

  Layer GetLayer() const;

private:
  Layer m_Layer;
};

enum class Player {
  Player1 = 1,
  Player2 = 2,
};

class LineObject : public KeyObject {
public:
  LineObject(beat_t beat, time_t time, key_t key, Player player, line_t line);
  virtual ~LineObject() override;

  Player GetPlayer() const;
  line_t GetLine() const;

private:
  Player m_Player;
  line_t m_Line;
};

class Note : public LineObject {
public:
  Note(beat_t beat, time_t time, key_t key, Player player, line_t line);
  Note(beat_t beat, time_t time, key_t key, Player player, line_t line,
       beat_t endBeat, time_t endTime);
  virtual ~Note() override;

  const std::unique_ptr<Object> &GetEnd() const;

private:
  std::unique_ptr<Object> m_End;
};

class Invisible : public LineObject {
public:
  Invisible(beat_t beat, time_t time, key_t key, Player player, line_t line);
  virtual ~Invisible() override;
};

class Bomb : public LineObject {
public:
  Bomb(beat_t beat, time_t time, key_t key, Player player, line_t line);
  virtual ~Bomb() override;
};

class Signatures {
public:
  void Set(measure_t measure, signature_t signature);
  signature_t Get(measure_t measure) const;

  beat_t GetBeatFromPos(pos_t pos) const;
  pos_t GetPosFromBeat(beat_t beat) const;

private:
  std::map<measure_t, signature_t> m_Signatures;
};
} // namespace rl::bms