
#include "../include/SquareDetector.h"
#include "../include/ContourDescriptor.h"

#include <opencv2/core.hpp> // Reduce include dependencies here

#include <iostream>

SquareDetector::SquareDetector(Parameters &p) : parameters(p) {}

std::unique_ptr<Detector> SquareDetector::create(Parameters &parameters)
{
  return std::unique_ptr<Detector>{new SquareDetector(parameters)};
}

static auto const claheParameters = cv::createCLAHE(1, cv::Size(8, 8));
static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
static auto const rect5x5Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

DetectionResult SquareDetector::detect(cv::Mat const &input)
{
  using ip = ImageProcessor;
  using cd = ContourDetector;

  cv::Mat equalized, temporary;
  auto processed = ip::filter(
      ip::toGray(input),
      {
          ip::equalize(claheParameters), // C ontrast L imited A daptive H istogram E qualization
          ip::cloneInto(equalized),      // Keep a copy of equalized image 4 later contour extraction
          ip::smooth(7),                 // Gauss
          ip::binarize(5, 1),            // Adaptive gaussian threshold binarization
          ip::open(rect3x3Kernel, 2),    // Morph open x times
          ip::close(rect3x3Kernel, 1)    // Morph close x times
      });

  auto const minimalSize = input.rows * input.cols * (1. / 100.);
  auto descriptors = cd::filter(
      cd::find(processed),
      {
          cd::removeIf(cd::areaSmallerThan(minimalSize)),     //
          cd::convexHull(),                                   //
          cd::approximateShape(cd::perimeterTimes(0.05)),     // Find straighter shapes
          cd::removeIf(cd::cornersDoesNotEqual(4)),           // 4 corners are compulory
          cd::removeIf(cd::sideLengthRatioLessThan(2. / 3.)), // Square like shapes only
          cd::sortBy(cd::biggestArea()),                      //
          cd::removeIfParent(),                               //
          cd::extractAndUnwarpFrom(equalized, 1.1f),          // Extract/unwarp image of contour + 10% margin
          cd::removeIf([](auto const &d)
                       { return d.image.empty(); }),
          cd::annotateWith({cd::dimensionString()}),
          /* cd::printTo(std::cout) */
      });

  std::for_each(descriptors.begin(), descriptors.end(), [](ContourDescriptor &descriptor)
                { descriptor.image = ip::filter(std::move(descriptor.image), {ip::binarize(45, 10)}); });

  return DetectionResult{std::move(temporary.empty() ? processed : temporary), std::move(descriptors)};
}
