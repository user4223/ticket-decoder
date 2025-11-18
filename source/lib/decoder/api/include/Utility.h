
#pragma once

#include "Result.h"

#include "lib/dip/include/ShapeProperties.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace decoder::api
{
  void dump(std::filesystem::path const &basePath, Result const &result);

  dip::utility::ShapeProperties getDrawProperties(Level level);

  char getCharacter(Level level);
}
