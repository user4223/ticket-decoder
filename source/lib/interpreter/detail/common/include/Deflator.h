
#pragma once

#include <vector>
#include <cstdint>

namespace interpreter::detail::common
{
  std::vector<std::uint8_t> deflate(std::vector<std::uint8_t> const &compressed);
}
