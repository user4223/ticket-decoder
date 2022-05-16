#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <string>

struct ContourDescriptor
{
  using ContourType = std::vector<cv::Point>;
  using AnnotatorType = std::function<std::tuple<cv::Point, std::string>(ContourDescriptor const &)>;

  enum class Level
  {
    Initial,
    Detected,
    Decoded
  };

  unsigned int id;
  unsigned int stepCount = 0;
  Level level = Level::Initial;

  ContourType contour;
  cv::Rect square;
  cv::Mat image;

  std::vector<AnnotatorType> annotators;

  std::string toString() const;

  static std::vector<ContourDescriptor> fromContours(std::vector<ContourType> &&contours);

  static std::vector<ContourDescriptor> fromRects(std::vector<cv::Rect> &&rects);

  static std::vector<ContourType> toContours(std::vector<ContourDescriptor> &&descriptors);
};