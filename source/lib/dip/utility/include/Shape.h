#pragma once

#include "Properties.h"

#include <opencv2/core.hpp>

namespace dip::utility
{
  void drawShape(cv::Mat &image, cv::Rect const &rectangle, Properties const &properties);

  void drawShape(cv::Mat &image, std::vector<cv::Point> const &shape, Properties const &properties);

  void drapRedShape(cv::Mat &image, std::vector<cv::Point> const &shape);
}
