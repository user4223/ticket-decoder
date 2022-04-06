#pragma once

#include <opencv2/core.hpp>

#include <vector>

struct DetectionResult
{
  using ContourType = std::vector<cv::Point>;

  cv::Mat input;
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Rect> objects;

  DetectionResult(cv::Mat &&input);

  cv::Mat visualize(cv::Mat const &destination);
};