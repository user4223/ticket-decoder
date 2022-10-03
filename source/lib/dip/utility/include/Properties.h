#pragma once

#include <opencv2/core.hpp>

namespace dip::utility
{
  struct Properties
  {
    cv::Scalar color;
    int thickness = 1;
  };
}
