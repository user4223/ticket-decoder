
#include "../include/Detector.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here
#include <filesystem>

Detector::Result::Result(cv::Mat const &i) : input(i) {}
Detector::Result::Result(cv::Mat const &i, std::vector<std::vector<cv::Point>> &&c) : input(i), contours(std::move(c)) {}
Detector::Result::Result(cv::Mat const &i, std::vector<cv::Rect> &&o) : input(i), objects(std::move(o)) {}

cv::Mat Detector::Result::visualize(cv::Mat &input)
{
  auto destination = (input.channels() == 3) ? input : [i = std::move(input)]()
  {
    cv::Mat transformed;
    cv::cvtColor(i, transformed, cv::COLOR_GRAY2RGB);
    return transformed;
  }();

  if (!contours.empty())
  {
    cv::drawContours(destination, contours, -1, cv::Scalar(0, 0, 255), 2);
  }

  if (!objects.empty())
  {
    std::for_each(objects.begin(), objects.end(), [&destination](cv::Rect const &o)
                  { cv::rectangle(destination, o, cv::Scalar(255, 0, 0), 2); });
  }

  return destination;
}

struct Detector::Internal
{
  std::unique_ptr<cv::CascadeClassifier> classifier;
};

auto findContours(cv::Mat const &input)
{
  auto contours = std::vector<std::vector<cv::Point>>{};
  cv::findContours(input, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  // Filter out smaller than minimal size
  {
    auto const minimalSize = input.rows * input.cols / 100.;
    auto iterator = std::remove_if(contours.begin(), contours.end(), [&minimalSize](auto const &contour)
                                   { return cv::contourArea(contour) < minimalSize; });
    contours.erase(iterator, contours.end());
  }

  // Calculate convex hull
  std::for_each(contours.begin(), contours.end(), [](auto &polygon)
                {
                  std::vector<cv::Point> output;
                  cv::convexHull(polygon, output);
                  polygon = output; });

  // Filter for non square shapes
  {
    auto iterator = std::remove_if(contours.begin(), contours.end(), [](auto const &contour)
                                   { 
                                   auto perimeter = cv::arcLength(contour, true);
                                   auto area = cv::contourArea(contour);
                                   return std::abs(perimeter - 4.*std::sqrt(area)) > 15.; });
    contours.erase(iterator, contours.end());
  }

  return contours;
}

auto detectObjects(cv::Mat const &input, cv::CascadeClassifier &classifier)
{
  std::vector<cv::Rect> objects;
  classifier.detectMultiScale(input, objects);
  return objects;
}

auto bullseyeDetector(cv::Mat const &input)
{
  auto const y_border = input.rows / 10;
  for (int y = 0; y < input.rows; ++y)
  {
    // detect 6 light regions separated from dark
    // use binary search from the middle to upper/lower image boundaries
    auto const x_border = input.cols / 10;
    auto it = cv::LineIterator(input, cv::Point(x_border, y), cv::Point(input.cols - x_border, y), 4);
    for (int i = 0; i < it.count; ++i)
    {
      auto const value = **it;
    }
  }
}

Detector::Detector() : internal(std::make_shared<Internal>())
{
  // std::string const file = "etc/aztec-classifier.xml";
  std::string const file = "etc/haarcascade_frontalface_default.xml";
  if (!std::filesystem::exists(file))
  {
    throw std::domain_error("Required classifier file not found: " + file);
  }

  internal->classifier = std::make_unique<cv::CascadeClassifier>(file);
}

Detector::Result Detector::detect(cv::Mat const &input)
{
  auto contours = findContours(input);
  // auto contours = detectObjects(input, *(internal->classifier));

  return Result{input, std::move(contours)};
}