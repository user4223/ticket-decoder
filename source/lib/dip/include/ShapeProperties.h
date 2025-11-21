#pragma once

#include <opencv2/core.hpp>

namespace dip
{
  struct ShapeProperties
  {
    cv::Scalar color;
    int thickness = 1;
  };
}
