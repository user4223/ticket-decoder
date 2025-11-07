#pragma once

#include <filesystem>
#include <optional>

namespace dip::detection::api
{
  struct DetectorOptions
  {
    std::optional<std::filesystem::path> classifierFile;
  };
}
