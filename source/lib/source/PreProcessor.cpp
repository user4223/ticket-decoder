#include "../include/PreProcessor.h"

#include <opencv2/opencv.hpp>

#include <iostream>

cv::Mat PreProcessor::process(cv::Mat input)
{
  cv::Mat gray;
  cv::cvtColor(input, gray, cv::COLOR_RGB2GRAY);

  // cv::GaussianBlur(gray, blurred, Size(55, 55), 0);
  // long double threshold = cv::threshold(blurred, binarized, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
  // std::cout << "Threshold: " << threshold << std::endl;

  cv::Mat binarized;
  cv::adaptiveThreshold(gray, binarized, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 0);

  auto const kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
  cv::morphologyEx(binarized, binarized, cv::MorphTypes::MORPH_CLOSE, kernel);
  // cv::morphologyEx(binarized, binarized, cv::MorphTypes::MORPH_OPEN, kernel);

  //{
  //	auto const center = Point2f{(gray.cols - 1) / 2.f, (gray.rows - 1) / 2.f};
  //	auto const rotation = cv::getRotationMatrix2D(center, angle += 1., 0.82);
  //	cv::warpAffine(binarized, toProcess, rotation, toProcess.size(), 1, 0, cv::Scalar(255));
  // }

  auto contours = std::vector<std::vector<cv::Point>>{};
  cv::findContours(binarized, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  auto const minimalContourSize = binarized.rows * binarized.cols / 20.;
  contours.erase(std::remove_if(contours.begin(), contours.end(), [&minimalContourSize](auto const &contour)
                                { return cv::contourArea(contour) < minimalContourSize; }),
                 contours.end());
  std::cout << "No contours: " << contours.size() << std::endl;

  cv::Mat output;
  cv::cvtColor(binarized, output, cv::COLOR_GRAY2RGB);
  cv::drawContours(output, contours, -1, cv::Scalar(0, 0, 255));
  return output;
}