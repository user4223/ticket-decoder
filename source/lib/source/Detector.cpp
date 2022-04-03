
#include "../include/Detector.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here
#include <filesystem>

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
    cv::drawContours(destination, contours, -1, cv::Scalar(0, 0, 255));
  }

  if (!objects.empty())
  {
    std::for_each(objects.begin(), objects.end(), [&destination](cv::Rect const &o)
                  { cv::rectangle(destination, o, cv::Scalar(255, 0, 0)); });
  }

  return destination;
}

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
    auto iterator = std::remove_if(contours.begin(), contours.end(), [&minimalSize](auto const &contour)
                                   { return cv::contourArea(contour) < minimalSize; });
    contours.erase(iterator, contours.end());
  }
  return contours;
}

auto detectObjects(cv::Mat const &input, cv::CascadeClassifier &classifier)
{
  std::vector<cv::Rect> objects;
  classifier.detectMultiScale(input, objects);

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

  return objects;
}

Detector::Detector() : internal(std::make_shared<Internal>())
{
  // std::string const file = "etc/aztec-classifier.xml";
  std::string const file = "/Users/saschalumma/.conan/data/opencv/4.5.5//_/_/package/6010c0000beddaabdc5491435386d457094e63ee/res/haarcascades/haarcascade_frontalface_alt.xml";
  if (!std::filesystem::exists(file))
  {
    throw std::domain_error("Required classifier file not found: " + file);
  }

  internal->classifier = std::make_unique<cv::CascadeClassifier>(file);
}

Detector::Result Detector::detect(cv::Mat const &input)
{
  // auto contours = findContours(input, input.rows * input.cols / 20.);
  auto contours = detectObjects(input, *(internal->classifier));

  return Result{input, std::move(contours)};
}