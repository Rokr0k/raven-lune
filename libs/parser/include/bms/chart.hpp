#pragma once

#include "header.hpp"
#include "objects.hpp"
#include <string>
#include <vector>

namespace rl::bms {
class Chart {
public:
  using total_t = float;

  Chart(const Header &header);

  Header::Rank GetRank() const;
  total_t GetTotal() const;
  const std::string &GetWAV(key_t key) const;
  const std::string &GetBMP(key_t key) const;

private:
  Header::Rank m_Rank;
  total_t m_Total;
  std::string m_Wavs[1296];
  std::string m_Bmps[1296];
  std::vector<std::unique_ptr<Object>> m_Objs;
};
} // namespace rl::bms