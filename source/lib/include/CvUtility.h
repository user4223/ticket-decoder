#pragma once

#include <memory>

#include <opencv2/core.hpp>

class Camera
{
public:
  static cv::Mat getImage(unsigned int device = 0);

  static void release();
};
