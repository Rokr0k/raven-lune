#pragma once

#include <string>
#include <vector>

namespace rl::bms {
class Header {
public:
  using level_t = int;

  enum class Difficulty {
    Easy = 1,
    Normal = 2,
    Hyper = 3,
    Another = 4,
    Insane = 5,
  };

  enum class Rank {
    VeryHard = 0,
    Hard = 1,
    Normal = 2,
    Easy = 3,
  };

  Header(const std::string &filename);

  const std::string &GetFilename() const;
  const std::string &GetGenre() const;
  const std::string &GetTitle() const;
  const std::string &GetArtist() const;
  const std::string &GetSubtitle() const;
  const std::string &GetSubartist() const;
  const std::string &GetStagefile() const;
  const std::string &GetBanner() const;
  level_t GetLevel() const;
  Difficulty GetDifficulty() const;
  Rank GetRank() const;

private:
  const std::vector<std::string> &GetUnusedLines() const;

  std::string m_Filename;
  std::string m_Genre;
  std::string m_Title;
  std::string m_Artist;
  std::string m_Subtitle;
  std::string m_Subartist;
  std::string m_Stagefile;
  std::string m_Banner;
  level_t m_Level;
  Difficulty m_Difficulty;
  Rank m_Rank;

  std::vector<std::string> m_UnusedLines;

  friend class Chart;
};
} // namespace rl::bms