
#include "../include/FileSystem.h"

#include <filesystem>
#include <random>
#include <iomanip>
#include <sstream>
#include <regex>
#include <fstream>

namespace utility
{
  std::string uniqueFilename(std::string directory, std::string extension)
  {
    return uniqueFilename(directory, "", extension);
  }

  std::string uniqueFilename(std::string directory, std::string postfix, std::string extension)
  {
    return ensureDirectory(directory) + "/" + uniqueBasename() + postfix + "." + extension;
  }

  std::string ensureDirectory(std::string name)
  {
    if (!std::filesystem::is_directory(name))
    {
      std::filesystem::create_directory(name);
    }
    return name;
  }

  static std::random_device random_device;
  static std::mt19937 generator(random_device());
  static std::uniform_int_distribution<> distribution(0);

  std::string uniqueBasename()
  {
    std::ostringstream os;
    os << std::setw(10) << std::setfill('0') << std::hex << distribution(generator);
    return os.str();
  }

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
