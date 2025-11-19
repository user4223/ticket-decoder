#pragma once

#include <opencv2/core.hpp>

#include <vector>

namespace detector::detail
{
  std::vector<cv::Point> normalizePointOrder(std::vector<cv::Point> &&contour);

  cv::Point2f centerOf(std::vector<cv::Point> const &contour);

  cv::Point round(cv::Point2d const &source);
}
