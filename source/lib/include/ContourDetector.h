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
  using PredicateType = std::function<bool(ContourDescriptor const &)>;

  static std::vector<ContourDescriptor> find(cv::Mat const &image);

  static std::vector<double> sideLengths(ContourDescriptor::ContourType const &contour, bool sort);

  static double maximumSideLengthRatio(ContourDescriptor::ContourType const &contour);

  static PredicateType areaSmallerThan(int size);

  static FilterType printTo(std::ostream &stream);

  static FilterType sortBy(std::function<bool(ContourDescriptor const &, ContourDescriptor const &)> comparator);

  static FilterType annotateWith(std::function<std::tuple<std::string, std::vector<std::string>>(int, ContourDescriptor &)> annotator);

  static FilterType removeIf(PredicateType predicate);

  static FilterType removeBeyond(int size);

  static FilterType convexHull();

  static FilterType approximateShape(std::function<double(ContourDescriptor const &)> epsilonSupplier);

  static std::vector<ContourDescriptor> filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters);
};