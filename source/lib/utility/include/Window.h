#pragma once

#include <opencv2/core.hpp>

namespace utility
{
  class Window
  {
  public:
    static void show(cv::Mat const &image);
  };
}
