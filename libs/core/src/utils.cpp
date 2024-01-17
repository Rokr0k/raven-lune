#include <rl/utils.hpp>
#include <filesystem>
#include <iconv.h>

namespace rl::utils {
std::vector<std::string> FindFile(const std::string &path) {
  std::vector<std::string> files;

  std::filesystem::path stem = std::filesystem::path{path}.stem();
  for (const std::filesystem::directory_entry &it :
       std::filesystem::directory_iterator{
           path.substr(0, path.find_last_of("/\\"))}) {
    if (it.is_regular_file() && it.path().stem() == stem) {
      files.push_back(it.path().string());
    }
  }

  return files;
}

std::u32string ConvertStringToU32(const std::string &utf8) {
  size_t utf8_len = utf8.size() + 2;
  size_t utf32_len = utf8_len * 4;

  char *utf8_str = new char[utf8_len]{};
  char *utf32_str = new char[utf32_len]{};
  char *utf8_ptr = utf8_str;
  char *utf32_ptr = utf32_str;

  std::copy(utf8.begin(), utf8.end(), utf8_str);

  iconv_t ic = iconv_open("UTF-32LE", "UTF-8");
  iconv(ic, &utf8_ptr, &utf8_len, &utf32_ptr, &utf32_len);
  iconv_close(ic);

  std::u32string utf32 = reinterpret_cast<char32_t *>(utf32_str);

  delete[] utf8_str;
  delete[] utf32_str;

  return utf32;
}
} // namespace rl::utils