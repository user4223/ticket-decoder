#pragma once

#include <filesystem>
#include <optional>

namespace detector::api
{
  struct DetectorOptions
  {
    std::optional<std::filesystem::path> classifierFile;
  };
}
