#pragma once

#include "ShapeProperties.h"

#include <opencv2/core.hpp>

namespace dip
{
  void drawShape(cv::Mat &image, cv::Rect const &rectangle, ShapeProperties const &properties);

  void drawShape(cv::Mat &image, std::vector<cv::Point> const &shape, ShapeProperties const &properties);

  void drawRedShape(cv::Mat &image, std::vector<cv::Point> const &shape);
}
