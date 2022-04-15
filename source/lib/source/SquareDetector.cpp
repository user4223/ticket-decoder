
#include "../include/SquareDetector.h"
#include "../include/ContourDescriptor.h"

#include <opencv2/core.hpp> // Reduce include dependencies here

#include <iostream>

std::unique_ptr<Detector> SquareDetector::create()
{
  return std::unique_ptr<Detector>{new SquareDetector()};
}

static auto const claheParameters = cv::createCLAHE(1, cv::Size(8, 8));
static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

DetectionResult SquareDetector::detect(cv::Mat const &input)
{
  using ip = ImageProcessor;
  using cd = ContourDetector;

  cv::Mat equalized;
  auto preProcessedImage = ip::filter(
      ip::toGray(input),
      {
          ip::equalize(claheParameters), // C ontrast L imited A daptive H istogram E qualization
          ip::cloneInto(equalized),      // Keep a copy of equalized image 4 later contour extraction
          ip::smooth(7),                 // Gauss
          ip::binarize(13, 1),           // Adaptive gaussian threshold binarization
          ip::open(rect3x3Kernel, 2),    // Morph open x times
          ip::close(rect3x3Kernel, 1)    // Morph close x times
      });

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto descriptors = cd::filter(
      cd::find(preProcessedImage),
      {
          cd::removeIf(cd::areaSmallerThan(minimalSize)),     //
          cd::convexHull(),                                   //
          cd::approximateShape(cd::perimeterTimes(0.05)),     // Find straighter shapes
          cd::removeIf(cd::cornersDoesNotEqual(4)),           // 4 corners are compulory
          cd::removeIf(cd::sideLengthRatioLessThan(2. / 3.)), // Square like shapes only
          cd::sortBy(cd::biggestArea()),                      //
          cd::removeIfParent(),                               //
          cd::extractAndUnwarpFrom(equalized, 1.1f),          // Extract/unwarp image of contour + 10% margin
          cd::annotateWith([](auto &d)
                           { return std::vector<std::string>{
                                 "area: " + std::to_string((int)cv::contourArea(d.contour))}; }),
          /* cd::printTo(std::cout) */
      });

  return DetectionResult{std::move(preProcessedImage), std::move(descriptors)};
}
