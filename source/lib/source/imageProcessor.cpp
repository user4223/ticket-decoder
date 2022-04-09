#include "../include/ImageProcessor.h"

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>

cv::Mat ImageProcessor::toGray(cv::Mat const &input)
{
  if (input.channels() == 1)
  {
    return input.clone();
  }

  cv::Mat gray;
  cv::cvtColor(input, gray, cv::COLOR_RGB2GRAY);
  return gray;
}

cv::Mat ImageProcessor::smooth(cv::Mat &&input, int const kernelSize)
{
  cv::Mat output;
  cv::GaussianBlur(input, output, cv::Size(kernelSize, kernelSize), 0);
  return output;
}

cv::Mat ImageProcessor::binarize(cv::Mat &&input, int const blockSize, int const substractFromMean)
{
  // Otsu is probably faster because simpler than adaptive threshold, but global theshold is
  // only useful when input image is in well defined range and does not have huge
  // light changes globally.
  // So we could make this a parameter to choose between camera image and properly
  // scanned image to make a compromise between speed and quality of input when reasonable.

  // cv::threshold(blurred, binarized, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

  cv::Mat output;
  cv::adaptiveThreshold(input, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, substractFromMean);
  return output;
}

cv::Mat ImageProcessor::equalize(cv::Mat &&input)
{
  cv::Mat output;
  cv::equalizeHist(input, output);
  return output;
}

cv::Mat ImageProcessor::equalize(cv::Mat &&input, cv::CLAHE &clahe)
{
  cv::Mat output;
  clahe.apply(input, output);
  return output;
}

cv::Mat ImageProcessor::open(cv::Mat &&input, cv::Mat const &kernel, int count)
{
  cv::Mat output;
  cv::morphologyEx(input, output, cv::MorphTypes::MORPH_OPEN, kernel, cv::Point(-1, -1), count);
  return output;
}

cv::Mat ImageProcessor::close(cv::Mat &&input, cv::Mat const &kernel, int count)
{
  cv::Mat output;
  cv::morphologyEx(input, output, cv::MorphTypes::MORPH_CLOSE, kernel, cv::Point(-1, -1), count);
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

cv::Mat ImageProcessor::process(cv::Mat &&input, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&input](auto const &filter)
                { input = filter(std::move(input)); });
  return input;
}
