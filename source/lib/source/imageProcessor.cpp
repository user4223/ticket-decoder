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

  cv::Mat output;
  cv::cvtColor(input, output, cv::COLOR_RGB2GRAY);
  return output;
}

cv::Mat ImageProcessor::rotate(cv::Mat const &input, float angle)
{
  auto const center = cv::Point2f{(input.cols - 1) / 2.f, (input.rows - 1) / 2.f};
  auto const rotation = cv::getRotationMatrix2D(center, angle, 0.82);
  cv::Mat output;
  cv::warpAffine(input, output, rotation, input.size(), 1, 0, input.channels() == 1 ? cv::Scalar(255) : cv::Scalar(255, 255, 255));
  return output;
}

ImageProcessor::FilterType ImageProcessor::smooth(int const kernelSize)
{
  auto const kernel = cv::Size(kernelSize, kernelSize);
  return [kernel](cv::Mat &&input)
  {
    cv::Mat output;
    cv::GaussianBlur(input, output, kernel, 0);
    return output;
  };
}

ImageProcessor::FilterType ImageProcessor::binarize(int const blockSize, int const substractFromMean)
{
  // Otsu is probably faster because simpler than adaptive threshold, but global theshold is
  // only useful when input image is in well defined range and does not have huge
  // light changes globally.
  // So we could make this a parameter to choose between camera image and properly
  // scanned image to make a compromise between speed and quality of input when reasonable.

  // cv::threshold(blurred, binarized, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
  return [blockSize, substractFromMean](cv::Mat &&input)
  {
    cv::Mat output;
    cv::adaptiveThreshold(input, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, substractFromMean);
    return output;
  };
}

ImageProcessor::FilterType ImageProcessor::equalize()
{
  return [](cv::Mat &&input)
  {
    cv::Mat output;
    cv::equalizeHist(input, output);
    return output;
  };
}

ImageProcessor::FilterType ImageProcessor::equalize(cv::Ptr<cv::CLAHE> const &clahe)
{
  return [clahe](cv::Mat &&input)
  {
    cv::Mat output;
    clahe->apply(input, output);
    return output;
  };
}

ImageProcessor::FilterType ImageProcessor::open(cv::Mat const &kernel, int count)
{
  auto const anchor = cv::Point(-1, -1);
  return [&kernel, anchor, count](cv::Mat &&input)
  {
    cv::Mat output;
    cv::morphologyEx(input, output, cv::MorphTypes::MORPH_OPEN, kernel, anchor, count);
    return output;
  };
}

ImageProcessor::FilterType ImageProcessor::close(cv::Mat const &kernel, int count)
{
  auto const anchor = cv::Point(-1, -1);
  return [&kernel, anchor, count](cv::Mat &&input)
  {
    cv::Mat output;
    cv::morphologyEx(input, output, cv::MorphTypes::MORPH_CLOSE, kernel, anchor, count);
    return output;
  };
}

ImageProcessor::FilterType ImageProcessor::cloneInto(cv::Mat &image)
{
  return [&](cv::Mat &&input)
  {
    image = input.clone();
    return std::move(input);
  };
}

cv::Mat ImageProcessor::filter(cv::Mat &&input, std::vector<FilterType> &&filters)
{
  // auto a = std::move(input);
  // auto b = cv::Mat(input.size(), input.type());
  std::for_each(filters.begin(), filters.end(), [&input](auto const &filter)
                { input = std::move(filter(std::move(input))); });
  return input;
}
