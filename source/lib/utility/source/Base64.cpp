
#include "../include/Base64.h"

namespace utility::base64
{
  static std::vector<int> createCharMap()
  {
    auto v = std::vector<int>(256, -1);
    for (int i = 0; i < 64; i++)
    {
      v["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }
    return v;
  }

  static auto const charMap = createCharMap();

  std::vector<std::uint8_t> decode(std::string const &encoded)
  {
    auto out = std::vector<std::uint8_t>{};
    int val = 0, valb = -8;
    for (std::uint8_t c : encoded)
    {
      auto const mapped = charMap[c];
      if (mapped == -1)
      {
        break;
      }
      val = (val << 6) + mapped;
      valb += 6;
      if (valb >= 0)
      {
        out.push_back(char((val >> valb) & 0xFF));
        valb -= 8;
      }
    }
    return out;
  }

}