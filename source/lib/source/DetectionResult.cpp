
#include "../include/DetectionResult.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <map>

DetectionResult::DetectionResult(cv::Mat &&i, std::vector<ContourDescriptor> &&d)
    : DetectionResult(std::move(i), std::optional<cv::Mat>{}, std::move(d)) {}

DetectionResult::DetectionResult(cv::Mat &&i, std::optional<cv::Mat> &&di, std::vector<ContourDescriptor> &&d)
    : image(std::move(i)), debugImage(std::move(di)), descriptors(std::move(d)) {}

static auto const cyan = cv::Scalar(255, 255, 0);
static auto const red = cv::Scalar(0, 0, 255);
static auto const yellow = cv::Scalar(0, 255, 255);
static auto const green = cv::Scalar(0, 255, 0);

static std::map<ContourDescriptor::Level, cv::Scalar> colorMap = {
    {ContourDescriptor::Level::Initial, red},
    {ContourDescriptor::Level::Detected, yellow},
    {ContourDescriptor::Level::Decoded, green}};

static cv::Scalar getColor(ContourDescriptor::Level level)
{
  auto const colorIterator = colorMap.find(level);
  return colorIterator == colorMap.end() ? cv::Scalar(0, 0, 255) : colorIterator->second;
}

cv::Mat DetectionResult::visualize(cv::Mat const &input, bool copyDetectedImage)
{
  auto destination = input.channels() == 3 ? input.clone() : [&input]()
  {
    cv::Mat transformed;
    cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
    return transformed;
  }();

  auto const coordinateThickness = 2;
  cv::putText(destination, "0x0", cv::Point(0, 25), cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness);
  cv::putText(destination, std::to_string(destination.cols) + "x0", cv::Point(destination.cols - 125, 25), cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness);
  cv::putText(destination, std::to_string(destination.cols) + "x" + std::to_string(destination.rows), cv::Point(destination.cols - 165, destination.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness);
  cv::putText(destination, "0x" + std::to_string(destination.rows), cv::Point(0, destination.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness);

  if (!descriptors.empty())
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &d)
                  {
                    if (d.contour.empty())
                      return;

                    auto const color = getColor(d.level);

                    if (copyDetectedImage && !d.image.empty())
                    {
                      auto const &part = d.image.channels() == 3 ? d.image : [&d]()
                      {
                        cv::Mat transformed;
                        cv::cvtColor(d.image, transformed, cv::COLOR_GRAY2RGB);
                        return transformed;
                      }();
                      part(cv::Rect(0, 0, d.square.width, d.square.height)).copyTo(destination(d.square));
                    }

                    cv::polylines(destination, d.contour, true, color, 2);
                    cv::rectangle(destination, d.square.tl(), d.square.br(), color, 2);

                    std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
                                  {
                                    auto const [position, text] = annotator(d);
                                    cv::putText(destination, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness); 
                                  }); });
  }

  return destination;
}