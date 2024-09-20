#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace utility::base64
{

  std::vector<std::uint8_t> decode(std::string const &in);

  std::string encode(std::vector<std::uint8_t> const &in);

  std::string encode(std::uint8_t const *const data, size_t size);

} // namespace utility
