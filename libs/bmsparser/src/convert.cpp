#include "table.hpp"
#include <bmsparser/convert.hpp>
#include <cstdint>

std::string bms::sjis_to_utf8(const std::string &input) {
  std::string output(3 * input.length(), ' ');
  size_t indexInput = 0, indexOutput = 0;

  while (indexInput < input.length()) {
    char arraySection = ((uint8_t)input[indexInput]) >> 4;

    size_t arrayOffset;
    if (arraySection == 0x8)
      arrayOffset = 0x100;
    else if (arraySection == 0x9)
      arrayOffset = 0x1100;
    else if (arraySection == 0xE)
      arrayOffset = 0x2100;
    else
      arrayOffset = 0;

    if (arrayOffset) {
      arrayOffset += (((uint8_t)input[indexInput]) & 0xf) << 8;
      indexInput++;
      if (indexInput >= input.length())
        break;
    }
    arrayOffset += (uint8_t)input[indexInput++];
    arrayOffset <<= 1;

    uint16_t unicodeValue = (shiftJIS_convTable[arrayOffset] << 8) |
                            shiftJIS_convTable[arrayOffset + 1];

    if (unicodeValue < 0x80) {
      output[indexOutput++] = unicodeValue;
    } else if (unicodeValue < 0x800) {
      output[indexOutput++] = 0xC0 | (unicodeValue >> 6);
      output[indexOutput++] = 0x80 | (unicodeValue & 0x3f);
    } else {
      output[indexOutput++] = 0xE0 | (unicodeValue >> 12);
      output[indexOutput++] = 0x80 | ((unicodeValue & 0xfff) >> 6);
      output[indexOutput++] = 0x80 | (unicodeValue & 0x3f);
    }
  }

  output.resize(indexOutput);
  return output;
}