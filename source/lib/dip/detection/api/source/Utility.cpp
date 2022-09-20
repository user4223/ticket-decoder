#include "../include/Utility.h"

#include "lib/dip/filtering/include/Transform.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace dip::detection::api
{
  static auto const blue = cv::Scalar(255, 0, 0);
  static auto const red = cv::Scalar(0, 0, 255);
  static auto const yellow = cv::Scalar(0, 255, 255);

  void visualize(cv::Mat &destination, std::vector<Descriptor> const &contours, bool overlayOutputImage)
  {
    destination = filtering::toColor(std::move(destination));

    auto const coordinateThickness = 2;
    cv::putText(destination, "0x0", cv::Point(0, 25), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);
    cv::putText(destination, std::to_string(destination.cols) + "x0", cv::Point(destination.cols - 125, 25), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);
    cv::putText(destination, std::to_string(destination.cols) + "x" + std::to_string(destination.rows), cv::Point(destination.cols - 180, destination.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);
    cv::putText(destination, "0x" + std::to_string(destination.rows), cv::Point(0, destination.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);

    if (!contours.empty())
    {
      std::for_each(contours.begin(), contours.end(), [&](auto const &d)
                    {
                    if (d.contour.empty()) 
                    {
                      return;
                    }

                    if (overlayOutputImage && !d.image.empty())
                    {
                      auto part = dip::filtering::toColor(d.image.clone());
                      part(cv::Rect(0, 0, d.square.width, d.square.height)).copyTo(destination(d.square));
                    }

                    cv::polylines(destination, d.contour, true, red, 2);
                    cv::rectangle(destination, d.square.tl(), d.square.br(), red, 2);

                    std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
                                  {
                                    auto const [position, text] = annotator(d);
                                    cv::putText(destination, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness); 
                                  }); });
    }
  }
}
