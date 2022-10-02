
#include "../include/Shape.h"
#include "../include/Color.h"

#include <opencv2/imgproc.hpp>

namespace dip::utility
{
  void drawShape(cv::Mat &image, cv::Rect const &rectangle, Properties const &properties)
  {
    cv::rectangle(image, rectangle.tl(), rectangle.br(), properties.color, properties.thickness);
  }

  void drawShape(cv::Mat &image, std::vector<cv::Point> const &shape, Properties const &properties)
  {
    if (shape.empty())
    {
      return;
    }
    cv::polylines(image, shape, true, properties.color, properties.thickness);
  }

  void drawRedShape(cv::Mat &image, std::vector<cv::Point> const &shape)
  {
    drawShape(image, shape, Properties{red, 1});
  }
}
