
#include "../include/Shape.h"
#include "../include/Color.h"

#include <opencv2/imgproc.hpp>

namespace dip::utility
{
  void drawShape(cv::Mat &image, cv::Scalar const &color, std::vector<cv::Point> const &shape)
  {
    if (shape.empty())
    {
      return;
    }
    cv::polylines(image, shape, true, color, 1);
  }

  void drapRedShape(cv::Mat &image, std::vector<cv::Point> const &shape)
  {
    drawShape(image, red, shape);
  }
}
