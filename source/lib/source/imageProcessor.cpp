#include "../include/ImageProcessor.h"
#include "../include/ImageDescriptor.h"

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>

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

ImageProcessor::FilterType ImageProcessor::rotate(float angle)
{
  return [angle](auto &&descriptor)
  {
    auto const center = cv::Point2f{(descriptor.image.cols - 1) / 2.f, (descriptor.image.rows - 1) / 2.f};
    auto const rotation = cv::getRotationMatrix2D(center, angle, 1.f /*0.82*/);
    cv::warpAffine(descriptor.image, descriptor.shaddow, rotation, descriptor.image.size(), cv::INTER_AREA, 0, descriptor.image.channels() == 1 ? cv::Scalar(255) : cv::Scalar(255, 255, 255));
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::smooth(int const kernelSize)
{
  auto const kernel = cv::Size(kernelSize, kernelSize);
  return [kernel](auto &&descriptor)
  {
    cv::GaussianBlur(descriptor.image, descriptor.shaddow, kernel, 0);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::binarize()
{
  // Otsu is probably faster because simpler than adaptive threshold, but global theshold is
  // only useful when input image is in well defined range and does not have huge
  // light changes globally.
  // So we could make this a parameter to choose between camera image and properly
  // scanned image to make a compromise between speed and quality of input when reasonable.
  return [](auto &&descriptor)
  {
    cv::threshold(descriptor.image, descriptor.shaddow, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::binarize(int const blockSize, int const substractFromMean)
{
  return [blockSize, substractFromMean](auto &&descriptor)
  {
    cv::adaptiveThreshold(descriptor.image, descriptor.shaddow, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, substractFromMean);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::equalize()
{
  return [](auto &&descriptor)
  {
    cv::equalizeHist(descriptor.image, descriptor.shaddow);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::edges(double threshold1, double threshold2, int aperture)
{
  return [=](auto &&descriptor)
  {
    cv::Canny(descriptor.image, descriptor.shaddow, threshold1, threshold2, aperture);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::equalize(cv::Ptr<cv::CLAHE> const &clahe)
{
  return [clahe](auto &&descriptor)
  {
    clahe->apply(descriptor.image, descriptor.shaddow);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::erode(cv::Mat const &kernel, int count)
{
  return [&kernel, count](auto &&descriptor)
  {
    cv::erode(descriptor.image, descriptor.shaddow, kernel, cv::Point(-1, -1), count, cv::BORDER_CONSTANT, cv::Scalar(255));
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::dilate(cv::Mat const &kernel, int count)
{
  return [&kernel, count](auto &&descriptor)
  {
    cv::dilate(descriptor.image, descriptor.shaddow, kernel, cv::Point(-1, -1), count, cv::BORDER_CONSTANT, cv::Scalar(255));
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::open(cv::Mat const &kernel, int count)
{
  auto const anchor = cv::Point(-1, -1);
  return [&kernel, anchor, count](auto &&descriptor)
  {
    cv::morphologyEx(descriptor.image, descriptor.shaddow, cv::MorphTypes::MORPH_OPEN, kernel, anchor, count);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::close(cv::Mat const &kernel, int count)
{
  auto const anchor = cv::Point(-1, -1);
  return [&kernel, anchor, count](auto &&descriptor)
  {
    cv::morphologyEx(descriptor.image, descriptor.shaddow, cv::MorphTypes::MORPH_CLOSE, kernel, anchor, count);
    return ImageDescriptor::swap(std::move(descriptor));
  };
}

ImageProcessor::FilterType ImageProcessor::cloneInto(cv::Mat &image)
{
  return [&](auto &&descriptor)
  {
    image = descriptor.image.clone();
    return std::move(descriptor);
  };
}

ImageDescriptor ImageProcessor::filter(ImageDescriptor &&descriptor, std::vector<FilterType> &&filters)
{
  return std::reduce(filters.begin(), filters.end(), std::move(descriptor), [](auto &&input, auto const &filter)
                     { 
                        auto output = filter(std::move(input)); 
                        output.stepCount++;
                        return std::move(output); });
}
