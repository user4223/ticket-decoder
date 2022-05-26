
#pragma once

#include <vector>
#include <cstdint>

class Deflator
{
public:
  static std::vector<std::uint8_t> deflate(std::vector<std::uint8_t> const &compressed);
};
