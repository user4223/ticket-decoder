#pragma once

#include <memory>

#include <opencv2/core.hpp>

namespace dip::utility
{
  cv::Mat readCamera(unsigned int device = 0);

  void releaseCamera();

}
