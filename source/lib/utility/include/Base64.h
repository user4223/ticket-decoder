#pragma once

#include <vector>
#include <string>

namespace utility::base64
{

  std::vector<std::uint8_t> decode(std::string const &in);

} // namespace utility
