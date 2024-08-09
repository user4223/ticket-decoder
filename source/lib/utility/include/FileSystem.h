#pragma once

#include <cstdint>
#include <vector>
#include <filesystem>

namespace utility
{
  // TODO Move into io module
  std::vector<std::uint8_t> readBinary(std::filesystem::path filePath);
}
