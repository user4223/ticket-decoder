#pragma once

#include "Descriptor.h"

#include <opencv2/core.hpp>

#include <vector>

namespace dip::detection::api
{
  void visualize(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box);

  void visualize(cv::Mat &image, std::vector<Descriptor> const &contours);

  void visualize(cv::Mat &image, std::vector<Descriptor> const &contours, bool overlayOutputImage);
}
