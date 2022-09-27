
#pragma once

#include "Result.h"

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>

namespace barcode::api
{

  void dump(std::filesystem::path const &basePath, Result const &result);

  void dump(std::filesystem::path const &basePath, std::vector<Result> const &results);

  void visualize(cv::Mat &image, Result const &result);

  void visualize(cv::Mat &image, std::vector<Result> const &results);

  void visualize(std::ostream &stream, Result const &result);

  void visualize(std::ostream &stream, std::vector<Result> const &results);

} // namespace barcode::api
