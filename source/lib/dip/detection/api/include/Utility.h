#pragma once

#include "Descriptor.h"

#include <opencv2/core.hpp>

#include <vector>

namespace dip::detection::api
{
  void visualize(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box);

  void visualize(cv::Mat &destination, Descriptor::ContourType const &contour);

  void visualize(cv::Mat &destination, Descriptor const &descriptor);
}
