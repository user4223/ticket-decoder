#pragma once

#include "ContourDescriptor.h"

#include <opencv2/core.hpp>

#include <vector>
#include <optional>

struct DetectionResult
{
  using ContourType = std::vector<cv::Point>;

  cv::Mat image;
  std::optional<cv::Mat> debugImage;
  std::vector<ContourDescriptor> descriptors;

  DetectionResult(cv::Mat &&input, std::vector<ContourDescriptor> &&descriptors);

  DetectionResult(cv::Mat &&input, std::optional<cv::Mat> &&debugImage, std::vector<ContourDescriptor> &&descriptors);

  cv::Mat visualize(cv::Mat const &destination, bool copyDetected);
};