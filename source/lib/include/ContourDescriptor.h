#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <string>
#include <optional>

struct ContourDescriptor
{
  using ContourType = std::vector<cv::Point>;
  using AnnotatorType = std::function<std::tuple<cv::Point, std::string>(ContourDescriptor const &)>;

  enum class Level
  {
    Located,
    Detected,
    Decoded
  };

  unsigned int id;
  Level level = Level::Located;

  ContourType contour;
  cv::Rect square;
  cv::Mat image;

  std::vector<AnnotatorType> annotators;

  std::string toString() const;

  ContourDescriptor clone() const
  {
    auto c = *this;
    c.image = image.clone();
    return c;
  }

  static std::vector<ContourDescriptor> fromContours(std::vector<ContourType> &&contours);

  static std::vector<ContourDescriptor> fromRects(std::vector<cv::Rect> &&rects);

  static std::vector<ContourType> toContours(std::vector<ContourDescriptor> &&descriptors);
};
