
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

static cv::Rect boundingSquare(ContourDescriptor::ContourType const &contour, float scale = 0.f)
{
  auto const rect = cv::boundingRect(contour);
  auto const center = (rect.br() + rect.tl()) * 0.5f;
  auto const length = rect.height > rect.width ? rect.height : rect.width;
  auto const half = length * 0.5f;
  auto const margin = length * scale;
  auto const margin2 = margin * 2.f;
  return cv::Rect(center.x - half - margin, center.y - half - margin, length + margin2, length + margin2);
}

static std::vector<cv::Point2f> toFloat(ContourDescriptor::ContourType const &contour)
{
  if (contour.size() != 4)
  {
    throw std::runtime_error("Expecting contour having exactly 4 corner points but got: " + std::to_string(contour.size()));
  }
  return std::vector<cv::Point2f>{
      (cv::Point2f)contour[0],
      (cv::Point2f)contour[1],
      (cv::Point2f)contour[2],
      (cv::Point2f)contour[3]};
}

static std::vector<cv::Point2f> toFloat(cv::Rect const &rect)
{
  return std::vector<cv::Point2f>{
      {(float)rect.x, (float)rect.y},
      {(float)(rect.x + rect.width), (float)rect.y},
      {(float)(rect.x + rect.width), (float)(rect.y + rect.height)},
      {(float)rect.x, (float)(rect.y + rect.height)}};
}

std::vector<cv::Point> ContourDetector::normalizePointOrder(std::vector<cv::Point> &&contour)
{
  if (contour.size() != 4)
  {
    throw std::runtime_error("Expecting contour having exactly 4 corner points but got: " + std::to_string(contour.size()));
  }

  std::sort(contour.begin(), contour.end(), [](auto const &a, auto const &b)
            { return a.x < b.x; });
  auto const [tl, bl] = contour[0].y > contour[1].y
                            ? std::make_tuple(contour[0], contour[1])
                            : std::make_tuple(contour[1], contour[0]);
  auto const [tr, br] = contour[2].y > contour[3].y
                            ? std::make_tuple(contour[2], contour[3])
                            : std::make_tuple(contour[3], contour[2]);

  return {tl, tr, br, bl};
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

ContourDetector::ComparatorType ContourDetector::compareArea(std::function<bool(double, double)> comparator)
{
  return [comparator](auto const &a, auto const &b)
  { return comparator(cv::contourArea(a.contour), cv::contourArea(b.contour)); };
}

ContourDetector::ComparatorType ContourDetector::smallestArea()
{
  return ContourDetector::compareArea([](auto a, auto b)
                                      { return a < b; });
}

ContourDetector::ComparatorType ContourDetector::biggestArea()
{
  return ContourDetector::compareArea([](auto a, auto b)
                                      { return a > b; });
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
    unsigned int counter = 0;
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto &d)
                  { d.id = counter++;
                    d.annotations = annotator(d); });
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

ContourDetector::FilterType ContourDetector::removeIfParent()
{
  return [](std::vector<ContourDescriptor> &&descriptors)
  {
    std::vector<unsigned int> ids;
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &outer)
                  { std::for_each(descriptors.begin(), descriptors.end(), [&](auto const &inner)
                                  { if (outer.id == inner.id) 
                                    { return; }

                                    if (std::any_of(inner.contour.begin(), inner.contour.end(), [&](auto const& point)
                                            { return cv::pointPolygonTest(outer.contour, point, false) >= 0; })) 
                                    { ids.push_back(outer.id); }; }); });

    auto iterator = std::remove_if(descriptors.begin(), descriptors.end(), [&](auto const &d)
                                   { return std::any_of(ids.begin(), ids.end(), [&](auto id)
                                                        { return d.id == id; }); });
    descriptors.erase(iterator, descriptors.end());
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
                  { auto const epsilon = epsilonSupplier(descriptor);
                    ContourDescriptor::ContourType output;
                    cv::approxPolyDP(descriptor.contour, output, epsilon, true);
                    descriptor.contour = std::move(output); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::extractAndUnwarpFrom(cv::Mat const &source, float scale)
{
  return [&source, scale](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&source, scale](auto &d)
                  { 
                    d.contour = normalizePointOrder(std::move(d.contour));
                    auto const moments = cv::moments(d.contour);
                    auto const cX = moments.m10 / moments.m00;
                    auto const cY = moments.m01 / moments.m00;                    

                    // TODO Put margin percent offset onto corner points
                    auto const contour = std::vector<cv::Point2f>{
                      cv::Point2f{d.contour[0].x * 1.f, d.contour[0].y * 1.f},
                      cv::Point2f{d.contour[1].x * 1.f, d.contour[1].y * 1.f},
                      cv::Point2f{d.contour[2].x * 1.f, d.contour[2].y * 1.f},
                      cv::Point2f{d.contour[3].x * 1.f, d.contour[3].y * 1.f}};

                    d.square = boundingSquare(d.contour, 0.f); 
                    d.image = cv::Mat(cv::Size(d.square.width, d.square.height), source.type());
                    auto const length = (float)d.square.width;
                    auto const transform = cv::getPerspectiveTransform(contour, std::vector<cv::Point2f>{
                      {0.f, length},    // tl
                      {length, length}, // tr
                      {length, 0.f},    // br
                      {0.f, 0.f}});     // bl
                    cv::warpPerspective(source, d.image, transform, d.image.size(), cv::INTER_NEAREST); });
    return std::move(descriptors);
  };
}

std::vector<ContourDescriptor> ContourDetector::filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&descriptors](auto const &filter)
                { descriptors = std::move(filter(std::move(descriptors))); });
  return std::move(descriptors);
}
