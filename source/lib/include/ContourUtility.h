#pragma once

#include <opencv2/core.hpp>

#include <vector>

struct ContourUtility
{

  static std::vector<cv::Point> normalizePointOrder(std::vector<cv::Point> &&contour);

  static cv::Point2f centerOf(std::vector<cv::Point> const &contour);

  static cv::Point round(cv::Point2d const &source);
};
