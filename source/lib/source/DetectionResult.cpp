
#include "../include/DetectionResult.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <map>

DetectionResult::DetectionResult(cv::Mat &&i)
    : input(std::move(i)) {}

DetectionResult::DetectionResult(cv::Mat &&i, std::vector<ContourDescriptor> &&d)
    : input(std::move(i)), descriptors(std::move(d)) {}

static std::map<ContourDescriptor::Level, cv::Scalar> colorMap = {
    {ContourDescriptor::Level::Initial, cv::Scalar(0, 0, 255)},
    {ContourDescriptor::Level::Detected, cv::Scalar(0, 255, 255)},
    {ContourDescriptor::Level::Interpreted, cv::Scalar(0, 255, 0)}};

static cv::Scalar getColor(ContourDescriptor::Level level)
{
  auto const colorIterator = colorMap.find(level);
  return colorIterator == colorMap.end() ? cv::Scalar(0, 0, 255) : colorIterator->second;
}

cv::Mat DetectionResult::visualize(cv::Mat const &input)
{
  auto destination = (input.channels() == 3) ? input.clone() : [&input]()
  {
    cv::Mat transformed;
    cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
    return transformed;
  }();

  if (!descriptors.empty())
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &d)
                  {
                    if (d.contour.empty())
                      return;

                    auto const color = getColor(d.level);

                    cv::polylines(destination, d.contour, true, color, 2);
                    cv::rectangle(destination, d.square.tl(), d.square.br(), color, 2);
                    cv::putText(destination, d.toString(), d.square.tl() + cv::Point2i(0, -10), cv::FONT_HERSHEY_PLAIN, 1., color, 2);

                    if (d.square.x < 0 || (d.square.x + d.square.width) >= destination.cols 
                    || d.square.y < 0 || (d.square.y + d.square.height) >= destination.rows)
                      return;

                    /* output(rect).copyTo(destination(rect)); */ });
  }

  if (!objects.empty())
  {
    std::for_each(objects.begin(), objects.end(), [&destination](auto const &o)
                  { cv::rectangle(destination, o, cv::Scalar(255, 0, 0), 2); });
  }

  return destination;
}