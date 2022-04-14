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
  using ComparatorType = std::function<bool(ContourDescriptor const &, ContourDescriptor const &)>;

  static std::vector<ContourDescriptor> find(cv::Mat const &image);

  static PredicateType areaSmallerThan(int size);

  static std::function<double(ContourDescriptor const &)> perimeterTimes(double factor);

  static PredicateType cornersDoesNotEqual(int size);

  static PredicateType sideLengthRatioLessThan(double ratio);

  static ComparatorType compareArea(std::function<bool(double, double)> comparator);

  static ComparatorType smallestArea();

  static ComparatorType biggestArea();

  static FilterType printTo(std::ostream &stream);

  static FilterType sortBy(ComparatorType comparator);

  static FilterType annotateWith(std::function<std::vector<std::string>(ContourDescriptor &)> annotator);

  static FilterType removeIf(PredicateType predicate);

  static FilterType removeIfParent();

  static FilterType removeBeyond(int size);

  static FilterType convexHull();

  static FilterType approximateShape(std::function<double(ContourDescriptor const &)> epsilonSupplier);

  static FilterType extractImage(cv::Mat const &source);

  static std::vector<ContourDescriptor> filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters);
};