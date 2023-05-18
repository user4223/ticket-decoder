
#include "../include/Base64.h"

#include <botan/base64.h>

namespace utility::base64
{

  std::vector<std::uint8_t> decode(std::string const &encoded)
  {
    auto const decoded = Botan::base64_decode(encoded.data(), encoded.size());
    return std::vector<std::uint8_t>(decoded.begin(), decoded.end());
  }

  std::string encode(std::vector<std::uint8_t> const &in)
  {
    auto const encoded = Botan::base64_encode(in);
    return encoded;
  }

  std::string encode(std::uint8_t const *const data, size_t size)
  {
    auto const encoded = Botan::base64_encode(data, size);
    return encoded;
  }

}