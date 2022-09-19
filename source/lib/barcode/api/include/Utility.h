
#pragma once

#include "Result.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace barcode::api
{

  void dump(std::filesystem::path const &basePath, Result const &result);

  void visualize(cv::Mat &image, Result const &result);

  void visualize(std::ostream &stream, Result const &result);

} // namespace barcode::api
