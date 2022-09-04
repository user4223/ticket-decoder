
#include "../include/Window.h"

#include <opencv2/highgui.hpp>

namespace utility
{
  static bool created = false;

  void Window::show(cv::Mat const &image)
  {
    if (!created)
    {
      cv::namedWindow("Output");
    }
    cv::imshow("Output", image);
  }
}
