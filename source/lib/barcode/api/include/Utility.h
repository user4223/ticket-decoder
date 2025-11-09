
#pragma once

#include "Result.h"

#include "lib/dip/utility/include/ShapeProperties.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace barcode::api
{
  void dump(std::filesystem::path const &basePath, Result const &result);

  dip::utility::ShapeProperties getDrawProperties(Level level);

  char getCharacter(Level level);
}
