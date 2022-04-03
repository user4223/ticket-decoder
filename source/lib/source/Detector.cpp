
#include "../include/Detector.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here

Detector::Result::Result(cv::Mat const &i, std::vector<std::vector<cv::Point>> &&c) : input(i), contours(std::move(c)) {}

struct Detector::Internal
{
  std::unique_ptr<cv::CascadeClassifier> classifier;
};

auto findContours(cv::Mat const &input, int minimalSize)
{
  auto contours = std::vector<std::vector<cv::Point>>{};
  cv::findContours(input, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  if (minimalSize > 0)
  {
    contours.erase(std::remove_if(contours.begin(), contours.end(), [&minimalSize](auto const &contour)
                                  { return cv::contourArea(contour) < minimalSize; }),
                   contours.end());
  }
  return contours;
}

auto findEquallyDistributedDarkLightZones(cv::Mat const &input)
{
  auto contours = std::vector<std::vector<cv::Point>>{};

  // auto const y_border = image.rows / 10;
  // for (int y = 0; y < image.rows; ++y)
  // {
  //   // detect 6 light regions separated from dark
  //   // use binary search from the middle to upper/lower image boundaries
  //   auto const x_border = image.cols / 10;
  //   auto it = cv::LineIterator(image, cv::Point(x_border, y), cv::Point(image.cols - x_border, y), 4);
  //   for (int i = 0; i < it.count; ++i)
  //   {
  //     auto const value = **it;
  //   }
  // }

  return contours;
}

Detector::Detector() : internal(std::make_shared<Internal>())
{
  internal->classifier = std::make_unique<cv::CascadeClassifier>();
}

Detector::Result Detector::detect(cv::Mat const &input)
{
  // auto contours = findContours(input, input.rows * input.cols / 20.);
  auto contours = findEquallyDistributedDarkLightZones(input);

  return Result{input, std::move(contours)};
}