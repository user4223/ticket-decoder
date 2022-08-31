#pragma once

#include <string>
#include <filesystem>

namespace utility
{
  std::string uniqueFilename(std::string directory, std::string postfix, std::string extension);

  std::string uniqueFilename(std::string directory, std::string extension);

  std::string ensureDirectory(std::string name);

  std::string uniqueBasename();

  std::vector<std::filesystem::path> scanForImages(std::filesystem::path directory);

  unsigned int safeDecrement(unsigned int &value);

  unsigned int safeDecrement(unsigned int &value, unsigned int decrement);

  unsigned int safeIncrement(unsigned int &value, unsigned int max);

  unsigned int safeIncrement(unsigned int &value, unsigned int increment, unsigned int max);

  unsigned int rotate(unsigned int &value, unsigned int max);
}
