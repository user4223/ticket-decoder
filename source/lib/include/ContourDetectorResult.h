#pragma once

#include "ContourDescriptor.h"

#include <opencv2/core.hpp>

#include <vector>
#include <optional>

struct ContourDetectorResult
{
  using ContourType = std::vector<cv::Point>;

  std::vector<ContourDescriptor> contours;
  std::optional<cv::Mat> debugImage;
  std::optional<std::vector<ContourDescriptor>> debugContours;

  ContourDetectorResult(std::vector<ContourDescriptor> &&descriptors);

  ContourDetectorResult(std::vector<ContourDescriptor> &&descriptors, std::optional<cv::Mat> &&debugImage, std::optional<std::vector<ContourDescriptor>> debugContours);

  cv::Mat visualize(cv::Mat const &image) const;
};