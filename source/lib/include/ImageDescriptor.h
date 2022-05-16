#pragma once

#include <opencv2/core.hpp>

struct ImageDescriptor
{
  unsigned int stepCount = 0;
  cv::Mat image;
  cv::Mat shaddow;

  static ImageDescriptor fromImage(cv::Mat &&image);

  static ImageDescriptor &&swap(ImageDescriptor &&descriptor);
};