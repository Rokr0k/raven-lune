#pragma once

#include <string>
#include <vector>

namespace rl::utils {
std::vector<std::string> FindFile(const std::string &path);

std::u32string ConvertStringToU32(const std::string &utf8);
} // namespace rl::utils