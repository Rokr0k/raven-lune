#include <bms/header.hpp>
#include <bms/convert.hpp>
#include <ctre.hpp>
#include <fstream>
#include <random>
#include <stack>
#include <string>

namespace rl::bms {
Header::Header(const std::string &filename)
    : m_Filename{filename}, m_Genre{"Unknown"}, m_Title{"Untitled"},
      m_Artist{"Unknown"}, m_Subtitle{}, m_Subartist{}, m_Stagefile{},
      m_Banner{}, m_Level{0}, m_Difficulty{Header::Difficulty::Normal},
      m_Rank{Header::Rank::Normal} {
  std::string parent = filename.substr(0, filename.find_last_of("/\\") + 1);

  int random;
  std::stack<bool> skip{{false}};
  std::random_device rd;
  std::mt19937 gen{rd()};

  std::ifstream input{std::string{filename}};
  std::string line;

  while (std::getline(input, line)) {
    line = ShiftJISToUTF8(line);

    if (auto m = ctre::match<R"(^\s*#RANDOM\s+(?<number>\d+)\s*$)",
                             ctre::case_insensitive>(line)) {
      std::uniform_int_distribution<int> dist{
          1, std::stoi(m.get<"number">().str())};
      random = dist(gen);
    } else if (auto m = ctre::match<R"(^\s*#IF\s+(?<number>\d+)\s*$)",
                                    ctre::case_insensitive>(line)) {
      skip.push(random != std::stoi(m.get<"number">().str()));
    } else if (auto m = ctre::match<R"(^\s*#ELSE\s*$)", ctre::case_insensitive>(
                   line)) {
      bool top = skip.top();
      skip.pop();
      skip.push(!top);
    } else if (auto m =
                   ctre::match<R"(^\s*#ENDIF\s*$)", ctre::case_insensitive>(
                       line)) {
      skip.pop();
    }

    if (skip.top())
      continue;

    if (auto m = ctre::match<R"(^\s*#GENRE\s+(?<genre>.*)\s*$)",
                             ctre::case_insensitive>(line)) {
      m_Genre = m.get<"genre">().view();
    } else if (auto m = ctre::match<R"(^\s*#TITLE\s+(?<title>.*)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Title = m.get<"title">().view();

      if (auto m = ctre::match<
              R"(^(?<title>.*)\s*(?:\[(?<subtitle>.*)\]|\((?<subtitle>.*)\)|\<(?<subtitle>.*)\>|\-(?<subtitle>.*)\-)$)">(
              m_Title)) {
        m_Title = m.get<"title">().view();
        m_Subtitle = "[" + m.get<"subtitle">().str() + "]";
      }
    } else if (auto m = ctre::match<R"(^\s*#ARTIST\s+(?<artist>.*)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Artist = m.get<"artist">().view();
    } else if (auto m = ctre::match<R"(^\s*#SUBTITLE\s+(?<subtitle>.*)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Subtitle = m.get<"subtitle">().view();
    } else if (auto m = ctre::match<R"(^\s*#SUBARTIST\s+(?<subartist>.*)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Subartist = m.get<"subartist">().view();
    } else if (auto m = ctre::match<R"(^\s*#STAGEFILE\s+(?<stagefile>.*)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Stagefile = parent + m.get<"stagefile">().str();
    } else if (auto m = ctre::match<R"(^\s*#BANNER\s+(?<banner>.*)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Banner = parent + m.get<"banner">().str();
    } else if (auto m = ctre::match<R"(^\s*#PLAYLEVEL\s+(?<level>\d+)\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Level = std::stoi(m.get<"level">().str());
    } else if (auto m =
                   ctre::match<R"(^\s*#DIFFICULTY\s+(?<difficulty>[1-5])\s*$)",
                               ctre::case_insensitive>(line)) {
      m_Difficulty = static_cast<Header::Difficulty>(
          std::stoi(m.get<"difficulty">().str()));
    } else if (auto m = ctre::match<R"(^\s*#RANK\s+(?<rank>[1-4])\s*$)",
                                    ctre::case_insensitive>(line)) {
      m_Rank = static_cast<Header::Rank>(std::stoi(m.get<"rank">().str()));
    } else if (ctre::match<R"(^\s*#.*$)">(line)) {
      m_UnusedLines.push_back(line);
    }
  }
}

const std::string &Header::GetFilename() const { return m_Filename; }

const std::string &Header::GetGenre() const { return m_Genre; }

const std::string &Header::GetTitle() const { return m_Title; }

const std::string &Header::GetArtist() const { return m_Artist; }

const std::string &Header::GetSubtitle() const { return m_Subtitle; }

const std::string &Header::GetSubartist() const { return m_Subartist; }

const std::string &Header::GetStagefile() const { return m_Stagefile; }

const std::string &Header::GetBanner() const { return m_Banner; }

Header::level_t Header::GetLevel() const { return m_Level; }

Header::Difficulty Header::GetDifficulty() const { return m_Difficulty; }

Header::Rank Header::GetRank() const { return m_Rank; }

const std::vector<std::string> &Header::GetUnusedLines() const {
  return m_UnusedLines;
}
} // namespace rl::bms