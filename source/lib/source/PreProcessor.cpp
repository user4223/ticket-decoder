#include "../include/PreProcessor.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <algorithm>

static cv::Mat toGray(cv::Mat &&input)
{
  cv::Mat output;
  cv::cvtColor(input, output, cv::COLOR_RGB2GRAY);
  return output;
}

static cv::Mat toBinary(cv::Mat &&input)
{
  cv::Mat output;
  cv::adaptiveThreshold(input, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 1);
  return output;
}

static cv::Mat smoothContours(cv::Mat &&input)
{
  auto const kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
  cv::Mat output;
  cv::morphologyEx(input, output, cv::MorphTypes::MORPH_OPEN, kernel);
  cv::morphologyEx(output, input, cv::MorphTypes::MORPH_CLOSE, kernel);
  return input;
}

cv::Mat PreProcessor::process(cv::Mat image)
{
  std::vector<std::function<cv::Mat(cv::Mat &&)>> filters;
  filters.push_back(toGray);
  filters.push_back(toBinary);
  // filters.push_back(smoothContours);

  std::for_each(filters.begin(), filters.end(), [&image](auto const &filter)
                { image = filter(std::move(image)); });

  auto const y_border = image.rows / 10;
  for (int y = 0; y < image.rows; ++y)
  {
    // detect 6 light regions separated from dark
    // use binary search from the middle to upper/lower image boundaries
    auto const x_border = image.cols / 10;
    auto it = cv::LineIterator(image, cv::Point(x_border, y), cv::Point(image.cols - x_border, y), 4);
    for (int i = 0; i < it.count; ++i)
    {
      auto const value = **it;
    }
  }

  // cv::GaussianBlur(gray, blurred, Size(55, 55), 0);
  // long double threshold = cv::threshold(blurred, binarized, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
  // std::cout << "Threshold: " << threshold << std::endl;

  //{
  //	auto const center = Point2f{(gray.cols - 1) / 2.f, (gray.rows - 1) / 2.f};
  //	auto const rotation = cv::getRotationMatrix2D(center, angle += 1., 0.82);
  //	cv::warpAffine(binarized, toProcess, rotation, toProcess.size(), 1, 0, cv::Scalar(255));
  // }

  // auto contours = std::vector<std::vector<cv::Point>>{};
  // cv::findContours(binarized, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  // auto const minimalContourSize = binarized.rows * binarized.cols / 20.;
  // contours.erase(std::remove_if(contours.begin(), contours.end(), [&minimalContourSize](auto const &contour)
  //                               { return cv::contourArea(contour) < minimalContourSize; }),
  //                contours.end());
  // std::cout << "No contours: " << contours.size() << std::endl;

  // cv::Mat output;
  // cv::cvtColor(binarized, output, cv::COLOR_GRAY2RGB);
  // cv::drawContours(output, contours, -1, cv::Scalar(0, 0, 255));
  return image;
}