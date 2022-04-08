
#include "../include/ContourDetector.h"
#include "../include/ContourDescriptor.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here
#include <filesystem>
#include <iostream>

ContourDetector::ContourDetector(ImageProcessor const &ip)
    : imageProcessor(ip) {}

std::unique_ptr<Detector> ContourDetector::create(ImageProcessor const &imageProcessor)
{
  return std::unique_ptr<Detector>{new ContourDetector(imageProcessor)};
}

using FilterType = std::function<std::vector<ContourDescriptor>(std::vector<ContourDescriptor> &&)>;

static std::vector<ContourDescriptor> print(std::vector<ContourDescriptor> &&descriptors, std::ostream &stream)
{
  std::for_each(descriptors.begin(), descriptors.end(), [](auto const &d)
                { std::cout << d.toString(); });
  std::cout << std::endl;
  return std::move(descriptors);
}

static std::vector<ContourDescriptor> annotate(std::vector<ContourDescriptor> &&descriptors, std::function<std::tuple<std::string, std::vector<std::string>>(int, ContourDescriptor &)> annotator)
{
  int counter = 0;
  std::for_each(descriptors.begin(), descriptors.end(), [&](auto &d)
                { auto [id, annotations] = annotator(counter++, d);
                  d.id = std::move(id);
                  d.annotations = std::move(annotations); });
  return std::move(descriptors);
}

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
                  ContourDescriptor::ContourType output;
                  cv::convexHull(descriptor.contour, output);
                  descriptor.contour = std::move(output); });
  return std::move(descriptors);
}

static std::vector<ContourDescriptor> sortBy(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &, ContourDescriptor const &)> comparator)
{
  std::sort(descriptors.begin(), descriptors.end(), comparator);
  return std::move(descriptors);
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
                  ContourDescriptor::ContourType output;
                  cv::approxPolyDP(descriptor.contour, output, epsilon, true);
                  descriptor.contour = std::move(output); });
  return std::move(descriptors);
}

static std::vector<double> calculateLengths(ContourDescriptor::ContourType const &contour, bool sort)
{
  auto lengths = std::vector<double>(contour.size());
  lengths[0] = cv::norm(contour[contour.size() - 1] - contour[0]);
  for (int c = 1; c < contour.size(); ++c)
  {
    lengths[c] = cv::norm(contour[c - 1] - contour[c]);
  }
  if (sort)
  {
    std::sort(lengths.begin(), lengths.end());
  }
  return lengths;
}

static std::vector<ContourDescriptor> process(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&descriptors](auto const &filter)
                { descriptors = filter(std::move(descriptors)); });
  return std::move(descriptors);
}

static auto const claheParameters = cv::createCLAHE(1, cv::Size(8, 8));
static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

DetectionResult ContourDetector::detect(cv::Mat const &input)
{
  using ip = ImageProcessor;
  auto preProcessedImage = imageProcessor.process(
      input,
      {[](auto &&input)
       { return ip::equalize(std::move(input), *claheParameters); },
       [](auto &&input)
       { return ip::smooth(std::move(input), 7); },
       [](auto &&input)
       { return ip::binarize(std::move(input), 13, 1); },
       [](auto &&input)
       { return ip::open(std::move(input), rect3x3Kernel, 2); },
       [](auto &&input)
       { return ip::close(std::move(input), rect3x3Kernel, 1); }});

  auto contours = std::vector<ContourDescriptor::ContourType>{};
  cv::findContours(preProcessedImage, contours, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto result = DetectionResult{std::move(preProcessedImage)};
  result.descriptors = process(
      ContourDescriptor::fromContours(std::move(contours)),
      {
          [&](auto &&descriptors)
          { return removeIf(std::move(descriptors), [&](auto const &d)
                            { return cv::contourArea(d.contour) < minimalSize; }); },
          [](auto &&descriptors)
          { return convexHull(std::move(descriptors)); },
          [](auto &&descriptors)
          { return approximateShape(std::move(descriptors), [](auto const &d)
                                    { return 0.05 * cv::arcLength(d.contour, true); }); },
          [](auto &&descriptors)
          { return removeIf(std::move(descriptors), [](auto const &d)
                            { return d.contour.size() != 4; }); },
          [](auto &&descriptors)
          { return removeIf(std::move(descriptors), [](auto const &d)
                            { 
                           auto const lengths = calculateLengths(d.contour, true);
                           return lengths[0] < (lengths[lengths.size() - 1] * (2./3.)); }); },
          [](auto &&descriptors)
          { return sortBy(std::move(descriptors), [](auto const &a, auto const &b)
                          { return cv::contourArea(a.contour) < cv::contourArea(b.contour); }); },
          [](auto &&descriptors)
          { return annotate(std::move(descriptors), [](int index, auto &d)
                            { return std::make_tuple(
                                  "#" + std::to_string(index + 1),
                                  std::vector<std::string>{"area: " + std::to_string(cv::contourArea(d.contour))}); }); },
          //[](auto &&descriptors)
          //{ return print(std::move(descriptors), std::cout); },
      });

  return result;
}
