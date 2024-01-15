#include <algorithm>
#include <bmsparser.hpp>
#include <fstream>
#include <map>
#include <sstream>
#include <stack>

using namespace bms;

static bool file_check(const std::string &file);

static Obj create_bgm(float fraction, int key);
static Obj create_bmp(float fraction, int key, int layer);
static Obj create_note(float fraction, int player, int line, int key, bool end);
static Obj create_inv(float fraction, int player, int line, int key);
static Obj create_bomb(float fraction, int player, int line, int damage);

Chart::Chart() {
  this->type = Type::Single;
  this->filename = "";
  this->genre = "";
  this->title = "";
  this->artist = "";
  this->subtitle = "";
  this->stagefile = "";
  this->banner = "";
  this->playLevel = 0;
  this->difficulty = 2;
  this->total = 160;
  this->rank = 2;
  this->wavs = new std::string[1296];
  this->bmps = new std::string[1296];
  this->signatures = new float[1000];
  for (int i = 0; i < 1000; i++) {
    this->signatures[i] = 1;
  }
  this->sectors.push_back(Sector(0, 0, 130, true));
}

Chart::Chart(const Chart &chart) {
  this->type = chart.type;
  this->filename = chart.filename;
  this->genre = chart.genre;
  this->title = chart.title;
  this->artist = chart.artist;
  this->subtitle = chart.subtitle;
  this->stagefile = chart.stagefile;
  this->banner = chart.banner;
  this->playLevel = chart.playLevel;
  this->difficulty = chart.difficulty;
  this->total = chart.total;
  this->rank = chart.rank;
  this->wavs = new std::string[1296];
  this->bmps = new std::string[1296];
  for (int i = 0; i < 1296; i++) {
    this->wavs[i] = chart.wavs[i];
    this->bmps[i] = chart.bmps[i];
  }
  this->signatures = new float[1000];
  for (int i = 0; i < 1000; i++) {
    this->signatures[i] = chart.signatures[i];
  }
  this->objs.assign(chart.objs.begin(), chart.objs.end());
  this->sectors.assign(chart.sectors.begin(), chart.sectors.end());
}

Chart::~Chart() {
  delete[] this->wavs;
  delete[] this->bmps;
  delete[] this->signatures;
}

Chart *bms::parseBMS(const std::string &file) {
  Chart *chart = new Chart;

  chart->filename = file;

  std::string parent = file.substr(0, file.find_last_of("/\\") + 1);

  std::vector<int> lnobj;
  std::map<int, bool> ln;
  float *bpms = new float[1296];
  float *stops = new float[1296];
  struct speedcore_t {
    float fraction;
    enum class Type {
      BPM,
      STP,
    } type;
    float value;
  };
  std::vector<speedcore_t> speedcore;
  int random;
  std::stack<bool> skip;
  skip.push(false);

  srand((unsigned int)time(NULL));

  std::ifstream input(file);
  std::string line;

  while (std::getline(input, line)) {
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

    if (line.empty() || line[0] != '#') {
      continue;
    }

    std::string content = line.substr(1);
    std::string header;
    std::string data;
    std::getline(std::stringstream(content) >> header >> std::ws, data);

    std::transform(header.begin(), header.end(), header.begin(), ::toupper);

    if (header == "RANDOM") {
      random = rand() % std::stoi(data) + 1;
    } else if (header == "IF") {
      skip.push(random != std::stoi(data));
    } else if (header == "ELSE") {
      bool top = skip.top();
      skip.pop();
      skip.push(!top);
    } else if (header == "ENDIF") {
      skip.pop();
    }

    if (skip.top()) {
      continue;
    }

    if (header == "GENRE") {
      chart->genre = data;
    } else if (header == "TITLE") {
      chart->title = data;
      const std::vector<std::pair<char, char>> brackets = {
          {'[', ']'}, {'{', '}'}, {'(', ')'}, {'<', '>'}, {'-', '-'},
      };
      for (const std::pair<char, char> &bracket : brackets) {
        size_t begin = chart->title.find_first_of(bracket.first);
        size_t end = chart->title.find_last_of(bracket.second);
        if (begin != std::string::npos && end != std::string::npos &&
            begin < end) {
          chart->subtitle =
              "[" + chart->title.substr(begin + 1, end - begin - 1) + "]";
          chart->title = chart->title.substr(0, begin);
          break;
        }
      }
    } else if (header == "ARTIST") {
      chart->artist = data;
    } else if (header == "SUBTITLE") {
      chart->subtitle = data;
    } else if (header == "SUBARTIST") {
      chart->subartist = data;
    } else if (header == "STAGEFILE") {
      chart->stagefile = parent + data;
    } else if (header == "BANNER") {
      chart->banner = parent + data;
    } else if (header == "PLAYLEVEL") {
      chart->playLevel = std::stoi(data);
    } else if (header == "DIFFICULTY") {
      chart->difficulty = std::stoi(data);
    } else if (header == "TOTAL") {
      chart->total = std::stof(data);
    } else if (header == "RANK") {
      chart->rank = std::stoi(data);
    } else if (header.length() == 5 && header.substr(0, 3) == "WAV") {
      int key = std::stoi(header.substr(3, 2), nullptr, 36);
      chart->wavs[key] = parent + data;
    } else if (header.length() == 5 && header.substr(0, 3) == "BMP") {
      int key = std::stoi(header.substr(3, 2), nullptr, 36);
      chart->bmps[key] = parent + data;
    } else if (header == "LNOBJ") {
      lnobj.push_back(std::stoi(data, nullptr, 36));
    } else if (header == "BPM") {
      chart->sectors[0].bpm = std::stof(data);
    } else if (header.length() == 5 && header.substr(0, 3) == "BPM") {
      int key = std::stoi(header.substr(3, 2), nullptr, 36);
      bpms[key] = std::stof(data);
    } else if (header.length() == 6 && header.substr(0, 4) == "STOP") {
      int key = std::stoi(header.substr(4, 2), nullptr, 36);
      stops[key] = std::stoi(data) / 192.0f;
    } else if (content[5] == ':') {
      int measure = std::stoi(content.substr(0, 3));
      if (content.substr(3, 2) == "02") {
        chart->signatures[measure] = std::stof(content.substr(6));
      } else {
        int channel = std::stoi(content.substr(3, 2), nullptr, 36);
        std::string objs = content.substr(6);
        unsigned long long l = objs.length() / 2;
        for (unsigned long long i = 0; i < l; i++) {
          int key = std::stoi(objs.substr(i * 2, 2), nullptr, 36);
          if (key) {
            float fraction = measure + (float)i / l;
            switch (channel) {
            case 1: // 01
              chart->objs.push_back(create_bgm(fraction, key));
              break;
            case 3: // 03
              speedcore.push_back(speedcore_t{
                  fraction,
                  speedcore_t::Type::BPM,
                  (float)std::stoi(objs.substr(i * 2, 2), nullptr, 16),
              });
              break;
            case 4: // 04
              chart->objs.push_back(create_bmp(fraction, key, 0));
              break;
            case 6: // 06
              chart->objs.push_back(create_bmp(fraction, key, -1));
              break;
            case 7: // 07
              chart->objs.push_back(create_bmp(fraction, key, 1));
              break;
            case 8: // 08
              speedcore.push_back(speedcore_t{
                  fraction,
                  speedcore_t::Type::BPM,
                  bpms[key],
              });
              break;
            case 9: // 09
              speedcore.push_back(speedcore_t{
                  fraction,
                  speedcore_t::Type::STP,
                  stops[key],
              });
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
              if (std::find(lnobj.begin(), lnobj.end(), key) == lnobj.end()) {
                chart->objs.push_back(create_note(fraction, key, channel / 36,
                                                  channel % 36, false));
              } else {
                chart->objs.push_back(create_note(fraction, key, channel / 36,
                                                  channel % 36, true));
                chart->objs.push_back(create_bgm(fraction, key));
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
              chart->objs.push_back(
                  create_inv(fraction, key, channel / 36 - 2, channel % 36));
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
              if (ln.find(channel) == ln.end()) {
                ln[channel] = false;
              }
              chart->objs.push_back(create_note(fraction, key, channel / 36 - 4,
                                                channel % 36, ln[channel]));
              ln[channel] = !ln[channel];
              break;
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
              chart->objs.push_back(
                  create_bomb(fraction, key, channel / 36 - 12, channel % 36));
              break;
            }
          }
        }
      }
    }
  }

  input.close();

  std::stable_sort(speedcore.begin(), speedcore.end(),
                   [](const speedcore_t &a, const speedcore_t &b) {
                     return a.fraction < b.fraction;
                   });
  for (speedcore_t &core : speedcore) {
    const Sector &last = chart->sectors.back();
    float pos = chart->frac2pos(core.fraction);
    float time = last.pos2time(pos);
    switch (core.type) {
    case speedcore_t::Type::BPM:
      chart->sectors.push_back(Sector(pos, time, core.value, true));
      break;
    case speedcore_t::Type::STP:
      chart->sectors.push_back(Sector(pos, time, 0, true));
      chart->sectors.push_back(
          Sector(pos, time + last.pos2time(core.value + last.pos) - last.time,
                 last.bpm, false));
      break;
    }
  }
  std::stable_sort(chart->objs.begin(), chart->objs.end(),
                   [](const Obj &a, const Obj &b) { return a.pos < b.pos; });
  for (Obj &note : chart->objs) {
    note.pos = chart->frac2pos(note.pos);
    note.time = chart->pos2time(note.pos);
  }

  delete[] bpms;
  delete[] stops;

  bool p2 = false;
  for (const Obj &obj : chart->objs) {
    switch (obj.type) {
    case Obj::Type::NOTE:
      if (obj.note.player > 1) {
        p2 = true;
      }
      break;
    case Obj::Type::INVISIBLE:
    case Obj::Type::BOMB:
      if (obj.misc.player > 1) {
        p2 = true;
      }
      break;
    default:
      break;
    }
  }

  if (!p2) {
    chart->type = Chart::Type::Single;
  } else {
    chart->type = Chart::Type::Dual;
  }

  return chart;
}

static bool file_check(const std::string &file) {
  std::ifstream stream(file);
  return stream.good();
}

float Chart::frac2pos(float frac) const {
  int measure = (int)frac;
  float f = frac - measure;
  float pos = f * this->signatures[measure];
  for (int m = 0; m < measure; m++) {
    pos += this->signatures[m];
  }
  return pos;
}

float Chart::pos2frac(float pos) const {
  int measure = 0;
  while (pos > 0) {
    pos -= this->signatures[measure++];
  }
  while (pos < 0) {
    pos += this->signatures[--measure];
  }
  return measure + pos / this->signatures[measure];
}

float Chart::pos2time(float pos) const {
  const std::vector<Sector>::const_reverse_iterator &i = std::find_if(
      this->sectors.crbegin(), this->sectors.crend(), [&pos](const Sector &a) {
        return a.pos < pos || (a.inclusive && a.pos == pos);
      });
  if (i != this->sectors.crend()) {
    return i->pos2time(pos);
  } else {
    return this->sectors.begin()->pos2time(pos);
  }
}

float Chart::time2pos(float time) const {
  const std::vector<Sector>::const_reverse_iterator &i = std::find_if(
      this->sectors.crbegin(), this->sectors.crend(), [&time](const Sector &a) {
        return a.time < time || (a.inclusive && a.time == time);
      });
  if (i != this->sectors.crend()) {
    return i->time2pos(time);
  } else {
    return this->sectors.begin()->time2pos(time);
  }
}

float Sector::pos2time(float pos) const {
  return this->time + (this->bpm > 0 ? (pos - this->pos) * 240 / this->bpm : 0);
}

float Sector::time2pos(float time) const {
  return this->pos + (time - this->time) / 240 * this->bpm;
}

static Obj create_bgm(float fraction, int key) {
  Obj obj;
  obj.type = Obj::Type::BGM;
  obj.pos = fraction;
  obj.bgm.key = key;
  return obj;
}

static Obj create_bmp(float fraction, int key, int layer) {
  Obj obj;
  obj.type = Obj::Type::BMP;
  obj.pos = fraction;
  obj.bmp.key = key;
  obj.bmp.layer = layer;
  return obj;
}

static Obj create_note(float fraction, int key, int player, int line,
                       bool end) {
  Obj obj;
  obj.type = Obj::Type::NOTE;
  obj.pos = fraction;
  obj.note.player = player;
  obj.note.line = line;
  obj.note.key = key;
  obj.note.end = end;
  return obj;
}

static Obj create_inv(float fraction, int key, int player, int line) {
  Obj obj;
  obj.type = Obj::Type::INVISIBLE;
  obj.pos = fraction;
  obj.misc.player = player;
  obj.misc.line = line;
  obj.misc.key = key;
  return obj;
}

static Obj create_bomb(float fraction, int damage, int player, int line) {
  Obj obj;
  obj.type = Obj::Type::BOMB;
  obj.pos = fraction;
  obj.misc.player = player;
  obj.misc.line = line;
  obj.misc.key = damage;
  return obj;
}
