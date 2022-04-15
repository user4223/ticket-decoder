#pragma once

#include "ContourDescriptor.h"

#include <opencv2/core.hpp>

#include <vector>

struct DetectionResult
{
  using ContourType = std::vector<cv::Point>;

  cv::Mat input;
  std::vector<ContourDescriptor> descriptors;

  DetectionResult(cv::Mat &&input, std::vector<ContourDescriptor> &&descriptors);

  cv::Mat visualize(cv::Mat const &destination);
};