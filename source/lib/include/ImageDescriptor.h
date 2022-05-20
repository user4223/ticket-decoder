#pragma once

#include <opencv2/core.hpp>

#include <optional>

struct ImageDescriptor
{
  unsigned int stepCount = 0;
  cv::Mat image;
  cv::Mat shaddow;
  std::optional<cv::Mat> debugImage;

  static ImageDescriptor fromImage(cv::Mat &&image);

  static ImageDescriptor &&swap(ImageDescriptor &&descriptor);
};
