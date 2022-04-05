
#include "../include/DetectionResult.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

DetectionResult::DetectionResult(cv::Mat const &i)
    : input(i) {}

cv::Mat DetectionResult::visualize(cv::Mat const &input)
{
  auto destination = (input.channels() == 3) ? input.clone() : [&input]()
  {
    cv::Mat transformed;
    cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
    return transformed;
  }();

  if (!contours.empty())
  {
    cv::drawContours(destination, contours, -1, cv::Scalar(0, 0, 255), 2);
  }

  if (!objects.empty())
  {
    std::for_each(objects.begin(), objects.end(), [&destination](auto const &o)
                  { cv::rectangle(destination, o, cv::Scalar(255, 0, 0), 2); });
  }

  return destination;
}