#pragma once

#include <opencv2/core.hpp>

namespace dip::utility
{
  void drawShape(cv::Mat &image, cv::Scalar const &color, std::vector<cv::Point> const &shape);

  void drapRedShape(cv::Mat &image, std::vector<cv::Point> const &shape);
}
