
#include "../include/Base64.h"

#include <botan/base64.h>

namespace utility::base64
{

  std::vector<std::uint8_t> decode(std::string const &encoded)
  {
    auto const decoded = Botan::base64_decode(encoded.data(), encoded.size());
    return std::vector<std::uint8_t>(decoded.begin(), decoded.end());
  }

}