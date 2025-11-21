
#include "../include/Shape.h"
#include "../include/Color.h"

#include <opencv2/imgproc.hpp>

namespace dip
{
  void drawShape(cv::Mat &image, cv::Rect const &rectangle, ShapeProperties const &properties)
  {
    if (rectangle == cv::Rect{})
    {
      return;
    }
    cv::rectangle(image, rectangle.tl(), rectangle.br(), properties.color, properties.thickness);
  }

  void drawShape(cv::Mat &image, std::vector<cv::Point> const &shape, ShapeProperties const &properties)
  {
    if (shape.empty())
    {
      return;
    }
    cv::polylines(image, shape, true, properties.color, properties.thickness);
  }

  void drawRedShape(cv::Mat &image, std::vector<cv::Point> const &shape)
  {
    drawShape(image, shape, ShapeProperties{red});
  }
}
