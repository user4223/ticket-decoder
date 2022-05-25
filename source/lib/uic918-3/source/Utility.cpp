
#include "../include/Utility.h"

namespace Utility
{

  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size)
  {
    return std::string{position, position += size};
  }

  template <typename T>
  T getNumeric(std::vector<std::uint8_t>::const_iterator &position)
  {
    auto value = std::vector<std::uint8_t>{position, position += sizeof(T)};
    // TODO This depends on endianess, call revers only when endianess change between source and destination
    std::reverse(value.begin(), value.end());
    return *(reinterpret_cast<T const *>(value.data()));
  }

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint16_t>(position);
  }

  std::vector<std::uint8_t> getBytes(std::vector<std::uint8_t>::const_iterator &position, std::size_t size)
  {
    return std::vector<std::uint8_t>{position, position += size};
  }
}