#pragma once

#include <opencv2/core/mat.hpp>

namespace input::camera
{
  cv::Mat readCamera(unsigned int device = 0);

  void releaseCamera();
}
