#include <SDL_stdinc.h>
#include <filesystem>
#include <rl/utils.hpp>

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
  char *utf32_buf =
      SDL_iconv_string("UTF-32LE", "UTF-8", utf8.c_str(), utf8.size() + 1);

  std::u32string utf32 = reinterpret_cast<char32_t *>(utf32_buf);

  SDL_free(utf32_buf);

  return utf32;
}
} // namespace rl::utils
