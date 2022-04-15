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

  unsigned int id;
  Level level = Level::Initial;

  ContourType contour;
  cv::Rect square;
  cv::Mat image;

  std::vector<std::string> annotations;

  std::string toString() const;

  static std::vector<ContourDescriptor> fromContours(std::vector<ContourType> &&contours);

  static std::vector<ContourDescriptor> fromRects(std::vector<cv::Rect> &&rects);

  static std::vector<ContourType> toContours(std::vector<ContourDescriptor> &&descriptors);
};