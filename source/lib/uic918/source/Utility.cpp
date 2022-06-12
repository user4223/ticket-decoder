
#include "../include/Utility.h"

#include <algorithm>
#include <bit>

namespace Utility
{

  std::string getAlphanumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t size)
  {
    return std::string{position, std::find(position, position += size, '\0')};
  }

  template <typename T>
  T getNumeric(std::vector<std::uint8_t>::const_iterator &position, std::size_t sourceLength = sizeof(T))
  {
    if (sizeof(T) < sourceLength)
    {
      throw std::runtime_error("Destination size must be equal or greater than source size");
    }

    auto source = std::vector<std::uint8_t>{position, position += sourceLength};
    auto destination = std::vector<std::uint8_t>(sizeof(T), 0);

    // TODO This depends on endianess, call revers only when endianess change between source and destination
    // if constexpr (std::endian::native == std::endian::big)
    // {
    //   throw std::runtime_error("Big endian machines not supported right now");
    // }
    // std::reverse(source.begin(), source.end());
    auto const offset = 0; // destination.size() - source.size(); // TODO Big endian has to offset here

    std::copy(source.begin(), source.end(), destination.begin() + offset);
    return *(reinterpret_cast<T const *>(destination.data()));
  }

  std::uint32_t getNumeric24(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint32_t>(position, 3);
  }

  std::uint16_t getNumeric16(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint16_t>(position);
  }

  std::uint8_t getNumeric8(std::vector<std::uint8_t>::const_iterator &position)
  {
    return getNumeric<std::uint8_t>(position);
  }

  std::vector<std::uint8_t> getBytes(std::vector<std::uint8_t>::const_iterator &position, std::size_t size)
  {
    return std::vector<std::uint8_t>{position, position += size};
  }
}