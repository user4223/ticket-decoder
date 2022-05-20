
#include "../include/ContourDetector.h"
#include "../include/ImageProcessor.h"
#include "../include/ContourUtility.h"

#include <numeric>

std::vector<ContourDescriptor::ContourType> ContourDetector::find(cv::Mat const &image)
{
  auto contours = std::vector<ContourDescriptor::ContourType>{};
  // auto hirarchy = std::vector<cv::Point>{};
  cv::findContours(image, contours, /*hirarchy, */ cv::RETR_LIST, cv::CHAIN_APPROX_TC89_L1);
  return contours;
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
        auto const position = d.square.tl() + cv::Point(d.square.width / 2, d.square.height / 2);
        if (d.contour.size() != 4)
        {
          return std::make_tuple(position, std::string("-"));
        }
        auto const top = cv::norm(d.contour[1] - d.contour[0]);
        auto const right = cv::norm(d.contour[2] - d.contour[1]);
        auto const bottom = cv::norm(d.contour[3] - d.contour[2]);
        auto const left = cv::norm(d.contour[0] - d.contour[3]);
        return std::make_tuple(position,
                               std::to_string((int)(top > bottom ? top : bottom)) + "x" + std::to_string((int)(left > right ? left : right)));
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
                  { d.contour = ContourUtility::normalizePointOrder(std::move(d.contour)); });
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
                  { 
                    auto temp = ImageProcessor::filter(
                      ImageDescriptor::fromImage(std::move(d.image)),
                      std::move(filter));
                    d.image = std::move(temp.image); });
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
    auto const innerPoint = borderPoint - orthogonalPitch;
    auto const outerPoint = borderPoint + orthogonalPitch;

    auto mostLeft = borderPoint;
    auto driftLine = cv::LineIterator(image, innerPoint, outerPoint, 8);
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
    auto const direction = cv::Point(mostLeft.x - innerPoint.x, mostLeft.y - innerPoint.y);
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

ContourDetector::FilterType ContourDetector::refineEdges(double const lengthFactor)
{
  return [=](std::vector<ContourDescriptor> &&descriptors)
  {
    std::for_each(descriptors.begin(), descriptors.end(), [lengthFactor](auto &d)
                  {
                    auto const base = d.square.tl();
                    auto const tl =  d.contour[0] - base;
                    auto const tr =  d.contour[1] - base;
                    auto const br =  d.contour[2] - base;
                    auto const bl =  d.contour[3] - base;
                    auto const [tl1, tr1] = moveLeft(d.image, tl, tr, lengthFactor);
                    auto const [tr2, br1] = moveLeft(d.image, tr1, br, lengthFactor);
                    auto const [br2, bl1] = moveLeft(d.image, br1, bl, lengthFactor);
                    auto const [bl2, tl2] = moveLeft(d.image, bl1, tl1, lengthFactor);
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
                    auto const center = ContourUtility::centerOf(d.contour);
                    auto const contour = std::vector<cv::Point2f>{
                      cv::Point2f{center.x - ((center.x - d.contour[0].x) * scale), center.y + ((d.contour[0].y - center.y) * scale)},
                      cv::Point2f{center.x + ((d.contour[1].x - center.x) * scale), center.y + ((d.contour[1].y - center.y) * scale)},
                      cv::Point2f{center.x + ((d.contour[2].x - center.x) * scale), center.y - ((center.y - d.contour[2].y) * scale)},
                      cv::Point2f{center.x - ((center.x - d.contour[3].x) * scale), center.y - ((center.y - d.contour[3].y) * scale)}};

                    auto const length = (float)(d.square.width > d.square.height ? d.square.width : d.square.height);
                    auto const transform = cv::getPerspectiveTransform(contour, std::vector<cv::Point2f>{
                      {0.f, 0.f},       // tl
                      {length, 0.f},    // tr
                      {length, length}, // br
                      {0.f, length}});  // bl

                    d.image = cv::Mat(cv::Size(length, length), source.type());
                    cv::warpPerspective(source, d.image, transform, d.image.size(), cv::INTER_AREA); });
    return std::move(descriptors);
  };
}

std::vector<ContourDescriptor> ContourDetector::filter(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters)
{
  return std::reduce(filters.begin(), filters.end(), std::move(descriptors), [](auto &&input, auto const &filter)
                     { 
                        auto output = filter(std::move(input)); 
                        //std::for_each(output.begin(), output.end(), [](auto &d) { d.stepCount++; }); 
                        return std::move(output); });
}
