
#include "../include/ContourDetector.h"
#include "../include/ImageProcessor.h"

#include <opencv2/imgproc.hpp>

std::vector<ContourDescriptor> ContourDetector::find(cv::Mat const &image)
{
  auto contours = std::vector<ContourDescriptor::ContourType>{};
  // auto hirarchy = std::vector<cv::Point>{};
  cv::findContours(image, contours, /*hirarchy, */ cv::RETR_LIST, cv::CHAIN_APPROX_TC89_L1);
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
  auto const [tl, bl] = contour[0].y < contour[1].y
                            ? std::make_tuple(contour[0], contour[1])
                            : std::make_tuple(contour[1], contour[0]);
  auto const [tr, br] = contour[2].y < contour[3].y
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

cv::Point2f ContourDetector::centerOf(ContourDescriptor::ContourType const &contour)
{
  auto const moments = cv::moments(contour);
  auto const cX = (float)(moments.m10 / moments.m00);
  auto const cY = (float)(moments.m01 / moments.m00);
  return cv::Point2f(cX, cY);
}

ContourDetector::PredicateType ContourDetector::cornersDoesNotEqual(int size)
{
  return [size](auto const &d)
  { return d.contour.size() != size; };
}

ContourDetector::PredicateType ContourDetector::emptyImage()
{
  return [](auto const &d)
  { return d.image.empty(); };
}

ContourDetector::PredicateType ContourDetector::boundingSquareOutsideOf(cv::Size const &size)
{
  return [&](auto const &d)
  {
    return d.square.empty()                                  //
           || d.square.x < 0                                 //
           || d.square.y < 0                                 //
           || (d.square.x + d.square.width) >= size.width    //
           || (d.square.y + d.square.height) >= size.height; //
  };
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

std::vector<ContourDescriptor::AnnotatorType> ContourDetector::dimensionString()
{
  return {
      [](auto &d)
      {
        return std::make_tuple(
            d.square.tl() + cv::Point(d.square.width / 2, d.square.height / 2),
            std::to_string(d.square.width) + "x" + std::to_string(d.square.height));
      },
  };
}

static std::string toString(cv::Point const &p)
{
  return std::to_string(p.x) + "," + std::to_string(p.y);
}

std::vector<ContourDescriptor::AnnotatorType> ContourDetector::coordinatesString()
{
  return {
      [](auto &d)
      { return std::make_tuple(d.contour[0], "tl: " + toString(d.contour[0])); },
      [](auto &d)
      { return std::make_tuple(d.contour[1], "tr: " + toString(d.contour[1])); },
      [](auto &d)
      { return std::make_tuple(d.contour[2], "br: " + toString(d.contour[2])); },
      [](auto &d)
      { return std::make_tuple(d.contour[3], "bl: " + toString(d.contour[3])); },
  };
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

ContourDetector::FilterType ContourDetector::annotateWith(std::vector<std::vector<ContourDescriptor::AnnotatorType>> &&annotators)
{
  return [annotators = std::move(annotators)](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(annotators.begin(), annotators.end(), [&](auto &a)
                  { std::for_each(descriptors.begin(), descriptors.end(), [&](auto &d)
                                  { d.annotators.insert(d.annotators.end(), std::begin(a), std::end(a)); }); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::annotateWith(std::vector<ContourDescriptor::AnnotatorType> &&annotators)
{
  return [annotators = std::move(annotators)](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&](auto &d)
                  { d.annotators.insert(d.annotators.end(), std::begin(annotators), std::end(annotators)); });
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

ContourDetector::FilterType ContourDetector::approximateShapeWith(std::function<double(ContourDescriptor const &)> epsilonSupplier)
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

ContourDetector::FilterType ContourDetector::normalizePointOrder()
{
  return [](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [](auto &d)
                  { d.contour = normalizePointOrder(std::move(d.contour)); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::determineBoundingSquareWith(float scale)
{
  return [=](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [=](auto &d)
                  { 
                    auto const rect = cv::boundingRect(d.contour);
                    auto const center = (rect.br() + rect.tl()) * 0.5f;
                    auto const length = scale * (rect.height > rect.width ? rect.height : rect.width);
                    auto const half = length * 0.5f;
                    d.square = cv::Rect(center.x - half, center.y - half, length, length); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::filterContourImages(std::vector<ImageProcessor::FilterType> &&filters)
{
  return [filter = std::move(filters)](std::vector<ContourDescriptor> &&descriptors) mutable
  {
    std::for_each(descriptors.begin(), descriptors.end(), [=](auto &d) mutable
                  { d.image = ImageProcessor::filter(std::move(d.image), std::move(filter)); });
    return std::move(descriptors);
  };
}

cv::Point getPositionAndContinue(cv::LineIterator &line, int const steps)
{
  auto pos = line.pos();
  for (auto i = 0; i < steps; ++i)
  {
    line++;
  }
  return pos;
}

double getMostFarDistance(cv::Mat &image, cv::LineIterator &&borderLine, cv::Point const &orthogonalPitch, int const stepSize)
{
  // std::tuple<cv::Point, cv::Point> result;
  auto mostFarDistance = 0.;
  for (auto s = 0; s < borderLine.count; s += stepSize)
  {
    auto const borderPoint = getPositionAndContinue(borderLine, stepSize);
    auto const outerPoint = borderPoint + orthogonalPitch;

    auto mostLeft = borderPoint;
    auto driftLine = cv::LineIterator(image, borderPoint, outerPoint, 8);
    auto counter = 0;
    for (auto t = 0; t < driftLine.count; t++, driftLine++)
    {
      if (**driftLine > 127)
      {
        mostLeft = driftLine.pos();
        counter = 0;
      }
      else
      {
        if (++counter > 15)
        {
          break;
        }
      }
    }
    auto const direction = cv::Point(mostLeft.x - borderPoint.x, mostLeft.y - borderPoint.y);
    auto const currentDistance = cv::norm(direction);
    if (currentDistance > mostFarDistance)
    {
      mostFarDistance = currentDistance;
      // result = std::make_tuple(mostLeft, outerPoint);
    }
  }
  // cv::arrowedLine(image, std::get<1>(result), std::get<0>(result), cv::Scalar(255));
  return mostFarDistance;
}

std::tuple<cv::Point, cv::Point> moveLeft(cv::Mat &image, cv::Point const &a, cv::Point const &b, double const lengthFactor)
{
  auto const direction = cv::Point2d((double)b.x - (double)a.x, (double)b.y - (double)a.y);
  auto const directionLength = cv::norm(direction);
  auto const normalizedDirection = direction / directionLength;
  auto const orthogonalDirection = cv::Point2d(normalizedDirection.y, -normalizedDirection.x);
  auto const orthogonalPitch = (cv::Point)(orthogonalDirection * directionLength * lengthFactor);

  auto stepSize = directionLength / 80;
  if (stepSize < 3)
  {
    stepSize = 3;
  }

  auto const cornerPitch = (cv::Point)(normalizedDirection * 0.4 * directionLength);
  auto const distanceA = getMostFarDistance(image, cv::LineIterator(image, a, a + cornerPitch, 8), orthogonalPitch, stepSize);
  auto const distanceB = getMostFarDistance(image, cv::LineIterator(image, b - cornerPitch, b, 8), orthogonalPitch, stepSize);

  return std::make_tuple(
      a + (cv::Point)(orthogonalDirection * (distanceA + 3.)),
      b + (cv::Point)(orthogonalDirection * (distanceB + 3.)));
}

ContourDetector::FilterType ContourDetector::refineEdges()
{
  return [](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [](auto &d)
                  {
                    auto const base = d.square.tl();
                    auto const tl =  d.contour[0] - base;
                    auto const tr =  d.contour[1] - base;
                    auto const br =  d.contour[2] - base;
                    auto const bl =  d.contour[3] - base;
                    auto const [tl1, tr1] = moveLeft(d.image, tl, tr, 0.05);
                    auto const [tr2, br1] = moveLeft(d.image, tr1, br, 0.05);
                    auto const [br2, bl1] = moveLeft(d.image, br1, bl, 0.05);
                    auto const [bl2, tl2] = moveLeft(d.image, bl1, tl1, 0.05);
                    d.contour[0] = tl2 + base;
                    d.contour[1] = tr2 + base;
                    d.contour[2] = br2 + base;
                    d.contour[3] = bl2 + base;
                    /**/ });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::extractFrom(cv::Mat const &source)
{
  return [&source](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&source](auto &d)
                  { 
                    d.image = cv::Mat(cv::Size(d.square.width, d.square.height), source.type());
                    source(cv::Rect(d.square.x, d.square.y, d.square.width, d.square.height))
                      .copyTo(d.image(cv::Rect(0,0, d.square.width, d.square.height))); });
    return std::move(descriptors);
  };
}

ContourDetector::FilterType ContourDetector::unwarpFrom(cv::Mat const &source, float scale)
{
  return [&source, scale](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [&source, scale](auto &d)
                  { 
                    auto const center = centerOf(d.contour);
                    auto const contour = std::vector<cv::Point2f>{
                      cv::Point2f{center.x - ((center.x - d.contour[0].x) * scale), center.y + ((d.contour[0].y - center.y) * scale)},
                      cv::Point2f{center.x + ((d.contour[1].x - center.x) * scale), center.y + ((d.contour[1].y - center.y) * scale)},
                      cv::Point2f{center.x + ((d.contour[2].x - center.x) * scale), center.y - ((center.y - d.contour[2].y) * scale)},
                      cv::Point2f{center.x - ((center.x - d.contour[3].x) * scale), center.y - ((center.y - d.contour[3].y) * scale)}};

                    auto const length = (float)(d.square.width > d.square.height ? d.square.width : d.square.height);
                    auto const transform = cv::getPerspectiveTransform(contour, std::vector<cv::Point2f>{
                      {0.f, length},    // tl
                      {length, length}, // tr
                      {length, 0.f},    // br
                      {0.f, 0.f}});     // bl

                    d.image = cv::Mat(cv::Size(length, length), source.type());
                    cv::warpPerspective(source, d.image, transform, d.image.size(), cv::INTER_AREA); });
    return std::move(descriptors);
  };
}

std::vector<ContourDescriptor> ContourDetector::filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  std::for_each(filters.begin(), filters.end(), [&descriptors](auto const &filter)
                { descriptors = std::move(filter(std::move(descriptors))); });
  return std::move(descriptors);
}
