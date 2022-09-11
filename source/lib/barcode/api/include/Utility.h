
#pragma once

#include "Result.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace barcode::api
{

  void dump(std::filesystem::path const &basePath, Result const &result, cv::Mat const &source);

} // namespace barcode::api
