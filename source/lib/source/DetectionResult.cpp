
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

                    auto const colorIterator = colorMap.find(d.level);
                    auto const &color = colorIterator == colorMap.end() ? cv::Scalar(0, 0, 255) : colorIterator->second;

                    cv::putText(destination, d.toString(), d.contour[0], cv::FONT_HERSHEY_SIMPLEX, 1., color, 2);
                    cv::polylines(destination, d.contour, true, color, 2);

                    auto rect = cv::boundingRect(d.contour);
                    if (rect.height > rect.width)
                    {
                      auto const center = (rect.br() + rect.tl()) * 0.5;
                      auto const f = rect.height * 0.5;
                      auto const s = rect.height * 0.05;
                      rect = cv::Rect(center.x - f - s, center.y - f - s, rect.height + 2. * s, rect.height + 2. * s);
                    }
                    else // since we add a margin in any case, we cannot skip the case width==height
                    {
                      auto const center = (rect.br() + rect.tl()) * 0.5;
                      auto const f = rect.width * 0.5;
                      auto const s = rect.width * 0.05;
                      rect = cv::Rect(center.x - f - s, center.y - f - s, rect.width + 2. * s, rect.width + 2. * s);
                    }
                    cv::rectangle(destination, rect.tl(), rect.br(), cv::Scalar(255, 0, 0), 2); });
  }

  if (!objects.empty())
  {
    std::for_each(objects.begin(), objects.end(), [&destination](auto const &o)
                  { cv::rectangle(destination, o, cv::Scalar(255, 0, 0), 2); });
  }

  return destination;
}