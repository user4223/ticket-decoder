
#include "../include/Utility.h"

#include <filesystem>
#include <random>
#include <iomanip>
#include <sstream>

namespace Utility
{

  std::string uniqueFilename(std::string directory, std::string extension)
  {
    return Utility::ensureDirectory(directory) + "/" + Utility::uniqueBasename() + "." + extension;
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

  bool toggleIf(bool condition, bool &value)
  {
    if (condition)
    {
      value = !value;
    }
    return value;
  }

}