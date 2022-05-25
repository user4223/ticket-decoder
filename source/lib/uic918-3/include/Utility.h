#pragma once

#include <string>
#include <vector>

namespace Utility
{

  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size);

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position);

}
