#include <algorithm>
#include <bms/chart.hpp>
#include <bms/convert.hpp>
#include <ctre.hpp>
#include <set>
#include <string>

namespace rl::bms {
Chart::Chart(const Header &header)
    : m_Rank{header.GetRank()}, m_Total{0}, m_Wavs{}, m_Bmps{} {
  std::string parent = header.GetFilename().substr(
      0, header.GetFilename().find_last_of("/\\") + 1);

  Signatures signatures;

  std::map<key_t, bpm_t> bpms;
  std::map<key_t, ratio_t> stops;

  struct Obj {
    pos_t pos;
    key_t channel;
    key_t key;
  };
  std::vector<Obj> objs;

  std::set<key_t> lnobjs;

  for (const auto &line : header.GetUnusedLines()) {
    if (auto m = ctre::match<R"(^\s*#TOTAL\s+(?<total>\d+(?:\.\d+)?)\s*$)",
                             ctre::case_insensitive>(line)) {
      m_Total = std::stof(m.get<"total">().str());
    } else if (auto m = ctre::match<
                   R"(^\s*#WAV(?<code>[0-9A-Z]{2})\s+(?<wav>.*)\s*$)",
                   ctre::case_insensitive>(line)) {
      key_t key = std::stoi(m.get<"code">().str(), nullptr, 36);
      m_Wavs[key] = parent + m.get<"wav">().str();
    } else if (auto m = ctre::match<
                   R"(^\s*#BMP(?<code>[0-9A-Z]{2})\s+(?<bmp>.*)\s*$)",
                   ctre::case_insensitive>(line)) {
      key_t key = std::stoi(m.get<"code">().str(), nullptr, 36);
      m_Bmps[key] = parent + m.get<"bmp">().str();
    } else if (auto m = ctre::match<R"(^\s*#LNOBJ\s+(?<code>[0-9A-Z]{2})\s*$)",
                                    ctre::case_insensitive>(line)) {
      lnobjs.insert(std::stoi(m.get<"code">().str(), nullptr, 36));
    } else if (auto m = ctre::match<R"(^\s*#BPM\s+(?<bpm>\d+(?:\.\d+)?)\s*$)",
                                    ctre::case_insensitive>(line)) {
      bpms.insert_or_assign(0, std::stof(m.get<"bpm">().str()));
    } else if (
        auto m = ctre::match<
            R"(^\s*#BPM(?<code>[0-9A-Z]{2})\s+(?<bpm>\d+(?:\.\d+)?)\s*$)",
            ctre::case_insensitive>(line)) {
      key_t key = std::stoi(m.get<"code">().str(), nullptr, 36);
      bpm_t bpm = std::stof(m.get<"bpm">().str());
      bpms.insert_or_assign(key, bpm);
    } else if (auto m = ctre::match<
                   R"(^\s*#STOP(?<code>[0-9A-Z]{2})\s+(?<stop>\d+)\s*$)",
                   ctre::case_insensitive>(line)) {
      key_t key = std::stoi(m.get<"code">().str(), nullptr, 36);
      ratio_t stop = std::stoi(m.get<"stop">().str()) / 48.f;
      stops.insert_or_assign(key, stop);
    } else if (
        auto m = ctre::match<
            R"(^\s*#(?<measure>\d{3})02:(?<signature>\d+(?:\.\d+)?)\s*$)",
            ctre::case_insensitive>(line)) {
      measure_t measure = std::stoi(m.get<"measure">().str());
      signature_t signature = std::stof(m.get<"signature">().str());
      signatures.Set(measure, signature);
    } else if (
        auto m = ctre::match<
            R"(^\s*#(?<measure>\d{3})(?<channel>[0-9A-Z]{2}):(?<codes>.*)\s*$)",
            ctre::case_insensitive>(line)) {
      measure_t measure = std::stoi(m.get<"measure">().str());
      key_t channel = std::stoi(m.get<"channel">().str(), nullptr, 36);
      auto codes = m.get<"codes">().view();
      for (std::size_t i = 0; i < codes.length() / 2; i++) {
        ratio_t ratio =
            static_cast<float>(i) / static_cast<int>(codes.length() / 2);
        auto code = codes.substr(i * 2, 2);
        key_t key = std::stoi(std::string{code}, nullptr, 36);
        if (key != 0) {
          objs.push_back(Obj{
              measure + ratio,
              channel,
              key,
          });
        }
      }
    }
  }

  std::stable_sort(
      objs.begin(), objs.end(),
      [](const Obj &lhs, const Obj &rhs) { return lhs.pos < rhs.pos; });

  if (bpms.find(0) != bpms.end()) {
    m_Objs.push_back(std::unique_ptr<Object>{new BPM{0, 0, bpms.at(0), true}});
  } else {
    m_Objs.push_back(std::unique_ptr<Object>{new BPM{0, 0, 130, true}});
  }

  for (const Obj &obj : objs) {
    beat_t beat = signatures.GetBeatFromPos(obj.pos);

    const BPM *bpm = dynamic_cast<BPM *>(
        std::find_if(m_Objs.rbegin(), m_Objs.rend(),
                     [beat](const std::unique_ptr<Object> &obj) {
                       BPM *bpm = dynamic_cast<BPM *>(obj.get());
                       if (bpm == nullptr)
                         return false;

                       return bpm->GetBeat() < beat ||
                              bpm->GetInclusive() && bpm->GetBeat() == beat;
                     })
            ->get());

    time_t time = bpm->GetTimeFromBeat(beat);

    switch (obj.channel) {
    case 1: // 01
      m_Objs.push_back(std::unique_ptr<Object>{new BGM{beat, time, obj.key}});
      break;
    case 3: // 03
      m_Objs.push_back(std::unique_ptr<Object>{
          new BPM{beat, time,
                  static_cast<bpm_t>(static_cast<int>(obj.key / 36) * 16 +
                                     obj.key % 36),
                  true}});
      break;
    case 4: // 04
      m_Objs.push_back(std::unique_ptr<Object>{
          new BMP{beat, time, obj.key, BMP::Layer::Base}});
      break;
    case 6: // 06
      m_Objs.push_back(std::unique_ptr<Object>{
          new BMP{beat, time, obj.key, BMP::Layer::Poor}});
      break;
    case 7: // 07
      m_Objs.push_back(std::unique_ptr<Object>{
          new BMP{beat, time, obj.key, BMP::Layer::Up}});
      break;
    case 8: // 08
      if (bpms.find(obj.key) != bpms.end())
        m_Objs.push_back(
            std::unique_ptr<Object>{new BPM{beat, time, bpms[obj.key], true}});
      else
        m_Objs.push_back(
            std::unique_ptr<Object>{new BPM{beat, time, 130, true}});
      break;
    case 9: // 09
      m_Objs.push_back(std::unique_ptr<Object>{new BPM{beat, time, 0, true}});
      m_Objs.push_back(std::unique_ptr<Object>{
          new BPM{beat, bpm->GetTimeFromBeat(beat + stops[obj.key]),
                  bpm->GetBPM(), false}});
      break;
    case 37: // 11
    case 38: // 12
    case 39: // 13
    case 40: // 14
    case 41: // 15
    case 42: // 16
    case 43: // 17
    case 44: // 18
    case 45: // 19
    case 73: // 21
    case 74: // 22
    case 75: // 23
    case 76: // 24
    case 77: // 25
    case 78: // 26
    case 79: // 27
    case 80: // 28
    case 81: // 29
      if (!lnobjs.contains(obj.key))
        m_Objs.push_back(std::unique_ptr<Object>{
            new Note{beat, time, obj.key, static_cast<Player>(obj.channel / 36),
                     obj.channel % 36}});
      else {
        auto it =
            std::find_if(m_Objs.rbegin(), m_Objs.rend(),
                         [&obj](const std::unique_ptr<Object> &o) {
                           const Note *note = dynamic_cast<Note *>(o.get());
                           if (note == nullptr)
                             return false;
                           return note->GetPlayer() ==
                                      static_cast<Player>(obj.channel / 36) &&
                                  note->GetLine() == obj.channel % 36;
                         });
        if (it != m_Objs.rend() &&
            dynamic_cast<Note *>(it->get())->GetEnd() == nullptr) {
          const Note *note = dynamic_cast<Note *>(it->get());
          std::unique_ptr<Object> newNote{
              new Note{note->GetBeat(), note->GetTime(), note->GetKey(),
                       note->GetPlayer(), note->GetLine(), beat, time}};
          *it = std::move(newNote);
        }
      }
      break;
    case 109: // 31
    case 110: // 32
    case 111: // 33
    case 112: // 34
    case 113: // 35
    case 114: // 36
    case 115: // 37
    case 116: // 38
    case 117: // 39
    case 145: // 41
    case 146: // 42
    case 147: // 43
    case 148: // 44
    case 149: // 45
    case 150: // 46
    case 151: // 47
    case 152: // 48
    case 153: // 49
      m_Objs.push_back(std::unique_ptr<Object>{new Invisible{
          beat, time, obj.key, static_cast<Player>(obj.channel / 36 - 2),
          obj.channel % 36}});
      break;
    case 181: // 51
    case 182: // 52
    case 183: // 53
    case 184: // 54
    case 185: // 55
    case 186: // 56
    case 187: // 57
    case 188: // 58
    case 189: // 59
    case 217: // 61
    case 218: // 62
    case 219: // 63
    case 220: // 64
    case 221: // 65
    case 222: // 66
    case 223: // 67
    case 224: // 68
    case 225: // 69
    {
      auto it =
          std::find_if(m_Objs.rbegin(), m_Objs.rend(),
                       [&obj](const std::unique_ptr<Object> &o) {
                         const Note *note = dynamic_cast<Note *>(o.get());
                         if (note == nullptr)
                           return false;
                         return note->GetPlayer() ==
                                    static_cast<Player>(obj.channel / 36 - 4) &&
                                note->GetLine() == obj.channel % 36;
                       });
      if (it != m_Objs.rend() &&
          dynamic_cast<Note *>(it->get())->GetEnd() == nullptr) {
        const Note *note = dynamic_cast<Note *>(it->get());
        std::unique_ptr<Object> newNote{
            new Note{note->GetBeat(), note->GetTime(), note->GetKey(),
                     note->GetPlayer(), note->GetLine(), beat, time}};
        *it = std::move(newNote);
      } else {
        m_Objs.push_back(std::unique_ptr<Object>{new Note{
            beat, time, obj.key, static_cast<Player>(obj.channel / 36 - 4),
            obj.channel % 36}});
      }
      break;
    }
    case 469: // D1
    case 470: // D2
    case 471: // D3
    case 472: // D4
    case 473: // D5
    case 474: // D6
    case 475: // D7
    case 476: // D8
    case 477: // D9
    case 505: // E1
    case 506: // E2
    case 507: // E3
    case 508: // E4
    case 509: // E5
    case 510: // E6
    case 511: // E7
    case 512: // E8
    case 513: // E9
      m_Objs.push_back(std::unique_ptr<Object>{new Bomb{
          beat, time, obj.key, static_cast<Player>(obj.channel / 36 - 12),
          obj.channel % 36}});
      break;
    }
  }

  std::stable_sort(m_Objs.begin(), m_Objs.end(),
                   [](const std::unique_ptr<Object> &lhs,
                      const std::unique_ptr<Object> &rhs) {
                     return lhs->GetBeat() < rhs->GetBeat();
                   });
}
} // namespace rl::bms
