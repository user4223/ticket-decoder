#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Utility
{

  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position);

  std::uint8_t getNumeric8(std::vector<std::uint8_t>::const_iterator &position);

  std::vector<std::uint8_t> getBytes(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);

}
