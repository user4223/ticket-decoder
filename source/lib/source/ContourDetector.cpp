
#include "../include/ContourDetector.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here
#include <filesystem>

static double perimeterAreaRate(std::vector<cv::Point> const &contour)
{
  auto const perimeter = cv::arcLength(contour, true);
  auto const area = cv::contourArea(contour);
  return std::abs(perimeter - 4. * std::sqrt(area));
}

auto findContours(cv::Mat const &input)
{
  auto contours = std::vector<std::vector<cv::Point>>{};
  cv::findContours(input, contours, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);

  // Filter out smaller than minimal size
  {
    auto const minimalSize = input.rows * input.cols / 150.;
    auto iterator = std::remove_if(contours.begin(), contours.end(), [&minimalSize](auto const &contour)
                                   { return cv::contourArea(contour) < minimalSize; });
    contours.erase(iterator, contours.end());
  }
  // Calculate convex hull
  {
    std::for_each(contours.begin(), contours.end(), [](auto &polygon)
                  {
                  std::vector<cv::Point> output;
                  cv::convexHull(polygon, output);
                  polygon = output; });
  }
  // Sort for the most square like of the remaining
  {
    std::sort(contours.begin(), contours.end(), [](auto const &a, auto const &b)
              { return perimeterAreaRate(a) < perimeterAreaRate(b); });
  }
  // Threshold out the most non-square shapes
  {
    auto const maximalShapeDescriptorVariance = 20.;
    auto iterator = std::find_if(contours.begin(), contours.end(), [&maximalShapeDescriptorVariance](auto const &c)
                                 { return perimeterAreaRate(c) > maximalShapeDescriptorVariance; });
    contours.erase(iterator, contours.end());
  }
  // Keep only the most square like shapes
  {
    if (contours.size() > 5)
    {
      contours.erase(contours.begin() + 5, contours.end());
    }
  }
  // Approximate clearer contours
  {
    std::for_each(contours.begin(), contours.end(), [](auto &contour)
                  {
                  auto const epsilon = 0.05 * cv::arcLength(contour, true);
                  std::vector<cv::Point> output;
                  cv::approxPolyDP(contour, output, epsilon, true);
                  contour = output; });
  }
  // Remove all remaining contours having less than 4 corners
  {
    auto iterator = std::find_if(contours.begin(), contours.end(), [](auto const &c)
                                 { return c.size() < 4; });
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

DetectionResult ContourDetector::detect(cv::Mat const &input)
{
  auto result = DetectionResult{input};
  result.contours = findContours(input);
  return result;
}
