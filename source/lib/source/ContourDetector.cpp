
#include "../include/ContourDetector.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here
#include <filesystem>
#include <iostream>

ContourDetector::ContourDetector(ImageProcessor const &ip)
    : imageProcessor(ip) {}

std::unique_ptr<Detector> ContourDetector::create(ImageProcessor const &imageProcessor)
{
  return std::unique_ptr<Detector>{new ContourDetector(imageProcessor)};
}

struct ContourDescriptor
{
  DetectionResult::ContourType contour;
  double perimeter;
  double area;
  double shapeDrift;

  void set(DetectionResult::ContourType &&c)
  {
    contour = std::move(c);
    perimeter = cv::arcLength(contour, true);
    area = cv::contourArea(contour);
    shapeDrift = std::abs(perimeter - 4. * std::sqrt(area));
  }

  static std::vector<ContourDescriptor> fromContours(std::vector<DetectionResult::ContourType> &&contours)
  {
    auto descriptors = std::vector<ContourDescriptor>{contours.size()};
    std::transform(contours.begin(), contours.end(), descriptors.begin(), [](auto &&c)
                   { 
                     auto descriptor = ContourDescriptor{};
                     descriptor.set(std::move(c));                  
                     return descriptor; });
    return descriptors;
  }

  static std::vector<DetectionResult::ContourType> toContours(std::vector<ContourDescriptor> &&descriptors)
  {
    auto contours = std::vector<DetectionResult::ContourType>{descriptors.size()};
    std::transform(descriptors.begin(), descriptors.end(), contours.begin(), [](auto &&d)
                   { return std::move(d.contour); });
    return contours;
  }
};

using FilterType = std::function<std::vector<ContourDescriptor>(std::vector<ContourDescriptor> &&)>;

static std::vector<ContourDescriptor> removeIf(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &)> predicate)
{
  auto iterator = std::remove_if(descriptors.begin(), descriptors.end(), predicate);
  descriptors.erase(iterator, descriptors.end());
  return std::move(descriptors);
}

static std::vector<ContourDescriptor> convexHull(std::vector<ContourDescriptor> &&descriptors)
{
  std::for_each(descriptors.begin(), descriptors.end(), [](auto &descriptor)
                {
                  DetectionResult::ContourType output;
                  cv::convexHull(descriptor.contour, output);
                  descriptor.set(std::move(output)); });
  return descriptors;
}

static std::vector<ContourDescriptor> sortBy(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &, ContourDescriptor const &)> comparator)
{
  std::sort(descriptors.begin(), descriptors.end(), comparator);
  return descriptors;
}

static std::vector<ContourDescriptor> maximumEntries(std::vector<ContourDescriptor> &&descriptors, int size)
{
  if (descriptors.size() > size)
  {
    descriptors.erase(descriptors.begin() + size, descriptors.end());
  }
  return std::move(descriptors);
}

static std::vector<ContourDescriptor> approximateShape(std::vector<ContourDescriptor> &&descriptors, std::function<double(ContourDescriptor const &)> epsilonSupplier)
{
  std::for_each(descriptors.begin(), descriptors.end(), [&](auto &descriptor)
                {
                  auto const epsilon = epsilonSupplier(descriptor);
                  DetectionResult::ContourType output;
                  cv::approxPolyDP(descriptor.contour, output, epsilon, true);
                  descriptor.set(std::move(output)); });
  return std::move(descriptors);
}

static std::vector<ContourDescriptor> process(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&descriptors](auto const &filter)
                { descriptors = filter(std::move(descriptors)); });
  return descriptors;
}

static auto const claheParameters = cv::createCLAHE(1, cv::Size(8, 8));
static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

DetectionResult ContourDetector::detect(cv::Mat const &input)
{
  auto preProcessedImage = imageProcessor.process(
      input,
      {[](auto &&input)
       { return ImageProcessor::equalize(std::move(input), *claheParameters); },
       [](auto &&input)
       { return ImageProcessor::smooth(std::move(input), 7); },
       [](auto &&input)
       { return ImageProcessor::binarize(std::move(input), 13, 1); },
       [](auto &&input)
       { return ImageProcessor::open(std::move(input), rect3x3Kernel, 2); },
       [](auto &&input)
       { return ImageProcessor::close(std::move(input), rect3x3Kernel, 1); }});

  auto contours = std::vector<DetectionResult::ContourType>{};
  cv::findContours(preProcessedImage, contours, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto result = DetectionResult{std::move(preProcessedImage)};
  result.contours = ContourDescriptor::toContours(process(
      ContourDescriptor::fromContours(std::move(contours)),
      {[&](auto &&descriptors)
       { return removeIf(std::move(descriptors), [&](auto const &d)
                         { return d.area < minimalSize; }); },
       [](auto &&descriptors)
       { return convexHull(std::move(descriptors)); },
       [](auto &&descriptors)
       { return removeIf(std::move(descriptors), [](auto const &d)
                         { return d.shapeDrift > 20.; }); },
       [](auto &&descriptors)
       { return sortBy(std::move(descriptors), [](auto const &a, auto const &b)
                       { return a.shapeDrift < b.shapeDrift; }); },
       [](auto &&descriptors)
       { return maximumEntries(std::move(descriptors), 5); },
       [](auto &&descriptors)
       { return approximateShape(std::move(descriptors), [](auto const &d)
                                 { return 0.05 * d.perimeter; }); },
       [](auto &&descriptors)
       { return removeIf(std::move(descriptors), [](auto const &d)
                         { return d.contour.size() < 4; }); }}));

  return result;
}
