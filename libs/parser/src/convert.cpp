#include <iconv.h>
#include <parser/convert.hpp>

namespace bms {
std::string sjis_to_utf8(std::string &input) {
  size_t sjis_len = input.size();
  size_t utf8_len = sjis_len * 2;

  char *sjis_str = new char[sjis_len];
  char *utf8_str = new char[utf8_len];
  char *sjis_ptr = sjis_str;
  char *utf8_ptr = utf8_str;

  std::copy(input.begin(), input.end(), sjis_str);

  iconv_t ic = iconv_open("UTF-8", "SHIFT_JIS");
  iconv(ic, &sjis_ptr, &sjis_len, &utf8_ptr, &utf8_len);
  iconv_close(ic);

  std::string output = utf8_str;

  delete[] sjis_str;
  delete[] utf8_str;

  return output;
}
} // namespace bms