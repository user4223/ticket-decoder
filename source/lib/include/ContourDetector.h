#pragma once

#include "ContourDescriptor.h"
#include "ImageProcessor.h"

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

  static std::vector<cv::Point> normalizePointOrder(std::vector<cv::Point> &&contour);

  static PredicateType areaSmallerThan(int size);

  static std::function<double(ContourDescriptor const &)> perimeterTimes(double factor);

  static PredicateType cornersDoesNotEqual(int size);

  static PredicateType emptyImage();

  static PredicateType boundingSquareOutOf(cv::Size const &size);

  static PredicateType sideLengthRatioLessThan(double ratio);

  static ComparatorType compareArea(std::function<bool(double, double)> comparator);

  static ComparatorType smallestArea();

  static ComparatorType biggestArea();

  static std::function<std::string(ContourDescriptor const &)> dimensionString();

  static FilterType printTo(std::ostream &stream);

  static FilterType sortBy(ComparatorType comparator);

  static FilterType annotateWith(std::vector<std::function<std::string(ContourDescriptor const &)>> &&annotators);

  static FilterType removeIf(PredicateType predicate);

  static FilterType removeIfParent();

  static FilterType removeBeyond(int size);

  static FilterType convexHull();

  static FilterType normalizePointOrder();

  static FilterType determineBoundingSquareWith(float scale);

  static FilterType approximateShapeWith(std::function<double(ContourDescriptor const &)> epsilonSupplier);

  static FilterType filterImages(std::vector<ImageProcessor::FilterType> &&filters);

  static FilterType refineEdges();

  static FilterType extractFrom(cv::Mat const &source);

  static FilterType unwarpFrom(cv::Mat const &source, float scale);

  static std::vector<ContourDescriptor> filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters);
};