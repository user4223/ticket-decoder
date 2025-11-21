
#pragma once

#include <vector>
#include <cstdint>

namespace interpreter::detail
{
  std::vector<std::uint8_t> deflate(std::vector<std::uint8_t> const &compressed);
}
