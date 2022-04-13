
#include "../include/ContourDetector.h"

#include <opencv2/imgproc.hpp>

std::vector<ContourDescriptor> ContourDetector::find(cv::Mat const &image)
{
  auto contours = std::vector<ContourDescriptor::ContourType>{};
  // auto hirarchy = std::vector<cv::Point>{};
  cv::findContours(image, contours, /*hirarchy, */ cv::RETR_TREE, cv::CHAIN_APPROX_TC89_L1);
  return ContourDescriptor::fromContours(std::move(contours));
}

static std::vector<double> sideLengths(ContourDescriptor::ContourType const &contour, bool sort)
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

ContourDetector::PredicateType ContourDetector::areaSmallerThan(int size)
{
  return [size](auto const &d)
  { return cv::contourArea(d.contour) < size; };
}

std::function<double(ContourDescriptor const &)> ContourDetector::perimeterTimes(double factor)
{
  return [factor](auto const &d)
  { return factor * cv::arcLength(d.contour, true); };
}

ContourDetector::PredicateType ContourDetector::cornersDoesNotEqual(int size)
{
  return [size](auto const &d)
  { return d.contour.size() != size; };
}

ContourDetector::PredicateType ContourDetector::sideLengthRatioLessThan(double ratio)
{
  return [ratio](auto const &d)
  {
    auto const lengths = sideLengths(d.contour, true);
    return lengths[0] < (lengths[lengths.size() - 1] * ratio);
  };
}

ContourDetector::ComparatorType ContourDetector::smallerArea()
{
  return [](auto const &a, auto const &b)
  { return cv::contourArea(a.contour) < cv::contourArea(b.contour); };
}

ContourDetector::FilterType ContourDetector::printTo(std::ostream &stream)
{
  return [&stream](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &d)
                  { stream << d.toString(); });
    stream << std::endl;
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::sortBy(ContourDetector::ComparatorType comparator)
{
  return [comparator](std::vector<ContourDescriptor> &&descriptors)
  {
    std::sort(descriptors.begin(), descriptors.end(), comparator);
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::annotateWith(std::function<std::vector<std::string>(ContourDescriptor &)> annotator)
{
  return [annotator](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto &d)
                  { d.annotations = annotator(d); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::removeIf(PredicateType predicate)
{
  return [predicate = std::move(predicate)](std::vector<ContourDescriptor> &&descriptors)
  {
    auto iterator = std::remove_if(descriptors.begin(), descriptors.end(), predicate);
    descriptors.erase(iterator, descriptors.end());
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::removeIfChild()
{
  return [](std::vector<ContourDescriptor> &&descriptors)
  {
    // TODO Implement me
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::removeBeyond(int size)
{
  return [size](std::vector<ContourDescriptor> &&descriptors)
  {
    if (descriptors.size() > size)
    {
      descriptors.erase(descriptors.begin() + size, descriptors.end());
    }
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::convexHull()
{
  return [](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [](auto &descriptor)
                  {
                  ContourDescriptor::ContourType output;
                  cv::convexHull(descriptor.contour, output);
                  descriptor.contour = std::move(output); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::approximateShape(std::function<double(ContourDescriptor const &)> epsilonSupplier)
{
  return [epsilonSupplier](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto &descriptor)
                  {
                  auto const epsilon = epsilonSupplier(descriptor);
                  ContourDescriptor::ContourType output;
                  cv::approxPolyDP(descriptor.contour, output, epsilon, true);
                  descriptor.contour = std::move(output); });
    return std::move(descriptors);
  };
}

std::vector<ContourDescriptor> ContourDetector::filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&descriptors](auto const &filter)
                { descriptors = std::move(filter(std::move(descriptors))); });
  return std::move(descriptors);
}
