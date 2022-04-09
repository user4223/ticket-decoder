
#include "../include/ContourDetector.h"

#include <opencv2/imgproc.hpp>

std::vector<ContourDescriptor> ContourDetector::find(cv::Mat const &image)
{
  auto contours = std::vector<ContourDescriptor::ContourType>{};
  cv::findContours(image, contours, cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);
  return ContourDescriptor::fromContours(std::move(contours));
}

std::vector<double> ContourDetector::sideLengths(ContourDescriptor::ContourType const &contour, bool sort)
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

double ContourDetector::maximumSideLengthRatio(ContourDescriptor::ContourType const &contour)
{
  auto const lengths = sideLengths(contour, true);
  return lengths[0] / lengths[lengths.size() - 1];
}

ContourDetector::FilterType ContourDetector::printTo(std::ostream &stream)
{
  return [&](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &d)
                  { stream << d.toString(); });
    stream << std::endl;
    return std::move(descriptors);
  };
}

std::vector<ContourDescriptor> ContourDetector::sortBy(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &, ContourDescriptor const &)> comparator)
{
  std::sort(descriptors.begin(), descriptors.end(), comparator);
  return std::move(descriptors);
}

std::vector<ContourDescriptor> ContourDetector::annotateWith(std::vector<ContourDescriptor> &&descriptors, std::function<std::tuple<std::string, std::vector<std::string>>(int, ContourDescriptor &)> annotator)
{
  int counter = 0;
  std::for_each(descriptors.begin(), descriptors.end(), [&](auto &d)
                { auto [id, annotations] = annotator(counter++, d);
                  d.id = std::move(id);
                  d.annotations = std::move(annotations); });
  return std::move(descriptors);
}

std::vector<ContourDescriptor> ContourDetector::removeIf(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &)> predicate)
{
  auto iterator = std::remove_if(descriptors.begin(), descriptors.end(), predicate);
  descriptors.erase(iterator, descriptors.end());
  return std::move(descriptors);
}

std::vector<ContourDescriptor> ContourDetector::removeBeyond(std::vector<ContourDescriptor> &&descriptors, int size)
{
  if (descriptors.size() > size)
  {
    descriptors.erase(descriptors.begin() + size, descriptors.end());
  }
  return std::move(descriptors);
}

std::vector<ContourDescriptor> ContourDetector::convexHull(std::vector<ContourDescriptor> &&descriptors)
{
  std::for_each(descriptors.begin(), descriptors.end(), [](auto &descriptor)
                {
                  ContourDescriptor::ContourType output;
                  cv::convexHull(descriptor.contour, output);
                  descriptor.contour = std::move(output); });
  return std::move(descriptors);
}

std::vector<ContourDescriptor> ContourDetector::approximateShape(std::vector<ContourDescriptor> &&descriptors, std::function<double(ContourDescriptor const &)> epsilonSupplier)
{
  std::for_each(descriptors.begin(), descriptors.end(), [&](auto &descriptor)
                {
                  auto const epsilon = epsilonSupplier(descriptor);
                  ContourDescriptor::ContourType output;
                  cv::approxPolyDP(descriptor.contour, output, epsilon, true);
                  descriptor.contour = std::move(output); });
  return std::move(descriptors);
}

std::vector<ContourDescriptor> ContourDetector::process(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&descriptors](auto const &filter)
                { descriptors = std::move(filter(std::move(descriptors))); });
  return std::move(descriptors);
}
