#pragma once

#include <string>
#include <filesystem>

namespace Utility
{

  std::string uniqueFilename(std::string directory, std::string extension);

  std::string ensureDirectory(std::string name);

  std::string uniqueBasename();

  std::vector<std::filesystem::path> scanForImages(std::filesystem::path directory);

  bool toggleIf(bool condition, bool &value);

  unsigned int safeDecrement(unsigned int &value);

  unsigned int safeIncrement(unsigned int &value, unsigned int max);

  unsigned int rotate(unsigned int &value, unsigned int max);
}
