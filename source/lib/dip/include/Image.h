#pragma once

#include <opencv2/core.hpp>

namespace dip::utility
{
  void copyTo(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box);

  std::vector<std::tuple<cv::Point, std::string>> getDimensionAnnotations(cv::Mat &image);
}
