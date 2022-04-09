#pragma once

#include "ContourDescriptor.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>
#include <ostream>
#include <functional>
#include <tuple>

class ContourDetector
{
public:
  using FilterType = std::function<std::vector<ContourDescriptor>(std::vector<ContourDescriptor> &&)>;

  static std::vector<ContourDescriptor> find(cv::Mat const &image);

  static std::vector<double> sideLengths(ContourDescriptor::ContourType const &contour, bool sort);

  static double maximumSideLengthRatio(ContourDescriptor::ContourType const &contour);

  static FilterType printTo(std::ostream &stream);

  static std::vector<ContourDescriptor> sortBy(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &, ContourDescriptor const &)> comparator);

  static std::vector<ContourDescriptor> annotateWith(std::vector<ContourDescriptor> &&descriptors, std::function<std::tuple<std::string, std::vector<std::string>>(int, ContourDescriptor &)> annotator);

  static std::vector<ContourDescriptor> removeIf(std::vector<ContourDescriptor> &&descriptors, std::function<bool(ContourDescriptor const &)> predicate);

  static std::vector<ContourDescriptor> removeBeyond(std::vector<ContourDescriptor> &&descriptors, int size);

  static std::vector<ContourDescriptor> convexHull(std::vector<ContourDescriptor> &&descriptors);

  static std::vector<ContourDescriptor> approximateShape(std::vector<ContourDescriptor> &&descriptors, std::function<double(ContourDescriptor const &)> epsilonSupplier);

  static std::vector<ContourDescriptor> process(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters);
};