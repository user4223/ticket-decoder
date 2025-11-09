#pragma once

#include <opencv2/core.hpp>

namespace dip::utility
{
  struct ShapeProperties
  {
    cv::Scalar color;
    int thickness = 1;
  };
}
