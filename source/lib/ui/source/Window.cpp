
#include "../include/Window.h"

#include <opencv2/highgui.hpp>

namespace ui
{
  static bool created = false;

  void showImage(cv::Mat const &image)
  {
    if (!created)
    {
      cv::namedWindow("Output");
    }
    cv::imshow("Output", image);
  }
}
