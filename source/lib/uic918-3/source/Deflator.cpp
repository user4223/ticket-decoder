
#include "../include/Deflator.h"

#include <zlib.h>
#include <stdexcept>
#include <string>

std::vector<std::uint8_t> Deflator::deflate(std::vector<std::uint8_t> const &compressed)
{
  unsigned long destinationSize = 1000;
  auto destination = std::vector<std::uint8_t>(destinationSize);
  auto const result = uncompress(destination.data(), &destinationSize, compressed.data(), compressed.size());
  if (result != Z_OK)
  {
    throw std::runtime_error(std::string("Failed to decompress message: ") + std::to_string(result));
  }
  destination.resize(destinationSize);
  return destination;
}