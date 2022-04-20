
#include "../include/DetectionResult.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <map>

DetectionResult::DetectionResult(cv::Mat &&i, std::vector<ContourDescriptor> &&d)
    : input(std::move(i)), descriptors(std::move(d)) {}

static std::map<ContourDescriptor::Level, cv::Scalar> colorMap = {
    {ContourDescriptor::Level::Initial, cv::Scalar(0, 0, 255)},
    {ContourDescriptor::Level::Detected, cv::Scalar(0, 255, 255)},
    {ContourDescriptor::Level::Decoded, cv::Scalar(0, 255, 0)}};

static cv::Scalar getColor(ContourDescriptor::Level level)
{
  auto const colorIterator = colorMap.find(level);
  return colorIterator == colorMap.end() ? cv::Scalar(0, 0, 255) : colorIterator->second;
}

cv::Mat DetectionResult::visualize(cv::Mat const &input, bool copyDetected)
{
  auto destination = input.channels() == 3 ? input.clone() : [&input]()
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
                    cv::putText(destination, d.toString(), d.square.tl() + cv::Point2i(0, -10), cv::FONT_HERSHEY_SIMPLEX, 1., color, 2);

                    if (!copyDetected || d.image.empty()) 
                      return;

                    auto const &part = d.image.channels() == 3 ? d.image : [&d](){
                      cv::Mat transformed;
                      cv::cvtColor(d.image, transformed, cv::COLOR_GRAY2RGB);
                      return transformed;
                    }();
                    part(cv::Rect(0, 0, d.square.width, d.square.height)).copyTo(destination(d.square)); });
  }

  return destination;
}