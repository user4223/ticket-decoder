#include "../include/ImageProcessor.h"

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>

static cv::Mat smooth7(cv::Mat &&input)
{
  cv::Mat output;
  cv::GaussianBlur(input, output, cv::Size(7, 7), 0);
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
  cv::adaptiveThreshold(input, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 13, 1);
  return output;
}

static cv::Mat equalize(cv::Mat &&input)
{
  cv::Mat output;
  cv::equalizeHist(input, output);
  return output;
}

static auto const claheHistogram = cv::createCLAHE(1, cv::Size(8, 8));

static cv::Mat equalizeClahe(cv::Mat &&input)
{
  cv::Mat output;
  claheHistogram->apply(input, output);
  return output;
}

static auto const rect7x7Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
static auto const rect5x5Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

static cv::Mat open3Close3(cv::Mat &&input)
{
  cv::Mat output;
  cv::morphologyEx(input, output, cv::MorphTypes::MORPH_OPEN, rect3x3Kernel, cv::Point(-1, -1), 2);
  cv::morphologyEx(output, input, cv::MorphTypes::MORPH_CLOSE, rect3x3Kernel, cv::Point(-1, -1), 1);
  return input;
}

static cv::Mat open5(cv::Mat &&input)
{
  cv::Mat output;
  cv::morphologyEx(input, output, cv::MorphTypes::MORPH_OPEN, rect5x5Kernel);
  return output;
}

static cv::Mat rotate(cv::Mat input, float angle)
{
  auto const center = cv::Point2f{(input.cols - 1) / 2.f, (input.rows - 1) / 2.f};
  auto const rotation = cv::getRotationMatrix2D(center, angle, 0.82);
  cv::Mat output;
  cv::warpAffine(input, output, rotation, input.size(), 1, 0, input.channels() == 1 ? cv::Scalar(255) : cv::Scalar(255, 255, 255));
  return output;
}

static cv::Mat process(cv::Mat const &input, std::vector<std::function<cv::Mat(cv::Mat &&)>> &&filters)
{
  cv::Mat gray;
  cv::cvtColor(input, gray, cv::COLOR_RGB2GRAY);
  std::for_each(filters.begin(), filters.end(), [&gray](auto const &filter)
                { gray = filter(std::move(gray)); });
  return gray;
}

cv::Mat ImageProcessor::preProcess(cv::Mat const &input)
{
  return process(input, {equalizeClahe,
                         // equalize, //
                         smooth7,
                         toBinary,
                         // open5, //
                         open3Close3});
}
