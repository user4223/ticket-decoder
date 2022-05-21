#pragma once

#include <string>

namespace Utility
{

  std::string uniqueFilename(std::string directory, std::string extension);

  std::string ensureDirectory(std::string name);

  std::string uniqueBasename();

  bool toggleIf(bool condition, bool &value);

  unsigned int safeDecrement(unsigned int &value);
}
