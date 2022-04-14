#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <string>

struct ContourDescriptor
{
  using ContourType = std::vector<cv::Point>;

  enum class Level
  {
    Initial,
    Detected,
    Interpreted
  };

  ContourType contour;
  unsigned int id;
  std::vector<std::string> annotations;
  Level level = Level::Initial;
  cv::Mat image;

  std::string toString() const;

  static std::vector<ContourDescriptor> fromContours(std::vector<ContourType> &&contours);

  static std::vector<ContourType> toContours(std::vector<ContourDescriptor> &&descriptors);
};