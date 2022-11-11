
#include "../include/Loader.h"

#include <filesystem>
#include <fstream>

namespace support
{
  std::vector<std::uint8_t> getData(std::string fileName)
  {
    auto const path = std::filesystem::path("..").append("uic918").append("etc").append(fileName);
    if (!std::filesystem::exists(path))
    {
      return {};
    }
    auto ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
    auto const size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    auto buffer = std::vector<std::uint8_t>(size);
    ifs.read(reinterpret_cast<char *>(buffer.data()), size);
    return buffer;
  }
}
