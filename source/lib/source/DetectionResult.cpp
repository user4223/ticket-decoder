
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

static cv::Rect boundingSquare(ContourDescriptor::ContourType const &contour, float scale)
{
  auto const rect = cv::boundingRect(contour);
  auto const center = (rect.br() + rect.tl()) * 0.5f;
  auto const length = rect.height > rect.width ? rect.height : rect.width;
  auto const half = length * 0.5f;
  auto const margin = length * scale;
  auto const margin2 = margin * 2.f;
  return cv::Rect(center.x - half - margin, center.y - half - margin, rect.height + margin2, rect.height + margin2);
}

static std::vector<cv::Point2f> toFloat(DetectionResult::ContourType const &contour)
{
  if (contour.size() != 4)
  {
    throw std::runtime_error("Expecting contour having exactly 4 corner points but got: " + std::to_string(contour.size()));
  }
  return std::vector<cv::Point2f>{
      (cv::Point2f)contour[0],
      (cv::Point2f)contour[1],
      (cv::Point2f)contour[2],
      (cv::Point2f)contour[3]};
}

static std::vector<cv::Point2f> toFloat(cv::Rect const &rect)
{
  return std::vector<cv::Point2f>{
      {(float)rect.x, (float)rect.y},
      {(float)(rect.x + rect.width), (float)rect.y},
      {(float)(rect.x + rect.width), (float)(rect.y + rect.height)},
      {(float)rect.x, (float)(rect.y + rect.height)}};
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
                    auto const rect = boundingSquare(d.contour, 0.05f);

                    cv::polylines(destination, d.contour, true, color, 2);                    
                    cv::rectangle(destination, rect.tl(), rect.br(), cv::Scalar(255, 0, 0), 2);
                    cv::putText(destination, d.toString(), rect.tl() + cv::Point2i(0, -10), cv::FONT_HERSHEY_SIMPLEX, 1., color, 2);

                    if (rect.x < 0 || (rect.x + rect.width) >= destination.cols || rect.y < 0 || (rect.y + rect.height) >= destination.rows) 
                      return;

                    auto const transform = cv::getPerspectiveTransform(toFloat(d.contour), toFloat(rect));
                    auto const output = destination.clone();
                    cv::warpPerspective(destination, output, transform, output.size()); 

                    auto const roi = destination(rect);
                    output(rect).copyTo(roi); });
  }

  if (!objects.empty())
  {
    std::for_each(objects.begin(), objects.end(), [&destination](auto const &o)
                  { cv::rectangle(destination, o, cv::Scalar(255, 0, 0), 2); });
  }

  return destination;
}