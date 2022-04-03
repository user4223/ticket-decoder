#include "../include/PreProcessor.h"

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>

static cv::Mat smooth(cv::Mat &&input)
{
  cv::Mat output;
  cv::GaussianBlur(input, output, cv::Size(5, 5), 0);
  return output;
}

static cv::Mat toBinary(cv::Mat &&input)
{
  // Otsu is probably faster because simpler than adaptive threshold, but global theshold is
  // only useful when input image is in well defined range and does not have huge
  // light changes globally.
  // So we could make this a parameter to choose between camera image and properly
  // scanned image to make a compromise between speed and quality of input when reasonable.

  // cv::threshold(blurred, binarized, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

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

cv::Mat PreProcessor::process(cv::Mat const &input)
{
  std::vector<std::function<cv::Mat(cv::Mat &&)>> filters;
  filters.push_back(smooth);
  filters.push_back(toBinary);
  // filters.push_back(smoothContours);

  cv::Mat gray;
  cv::cvtColor(input, gray, cv::COLOR_RGB2GRAY);

  std::for_each(filters.begin(), filters.end(), [&gray](auto const &filter)
                { gray = filter(std::move(gray)); });

  //{
  //	auto const center = Point2f{(gray.cols - 1) / 2.f, (gray.rows - 1) / 2.f};
  //	auto const rotation = cv::getRotationMatrix2D(center, angle += 1., 0.82);
  //	cv::warpAffine(binarized, toProcess, rotation, toProcess.size(), 1, 0, cv::Scalar(255));
  // }

  return gray;
}