
#include "../include/FileSystem.h"

#include <fstream>

namespace utility
{
  std::vector<std::uint8_t> readBinary(std::filesystem::path filePath)
  {
    if (!std::filesystem::is_regular_file(filePath))
    {
      throw std::runtime_error(std::string("Path to load raw data from does not exist or does not point to a file: ") + filePath.string());
    }

    auto ifs = std::ifstream(filePath, std::ios::binary | std::ios::ate);
    auto const size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    auto buffer = std::vector<std::uint8_t>(size);
    ifs.read(reinterpret_cast<char *>(buffer.data()), size);
    return buffer;
  }
}
