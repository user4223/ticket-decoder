#pragma once

#include "Descriptor.h"

#include <opencv2/core.hpp>

namespace dip::detection::api
{
  void visualize(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box);
}
