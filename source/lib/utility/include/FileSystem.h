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
}
