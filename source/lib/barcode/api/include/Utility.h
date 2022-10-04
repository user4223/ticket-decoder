
#pragma once

#include "Result.h"

#include "lib/dip/utility/include/Properties.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace barcode::api
{
  void dump(std::filesystem::path const &basePath, Result const &result);

  dip::utility::Properties getDrawProperties(Level level);

  char getCharacter(Level level);
}
