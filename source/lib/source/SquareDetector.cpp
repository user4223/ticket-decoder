
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

  auto preProcessedImage = ip::filter(
      ip::toGray(input),
      {
          ip::equalize(claheParameters), // Contrast Limited Adaptive Histogram Equalization
          ip::smooth(7),                 // Gauss
          ip::binarize(13, 1),           // Adaptive Gaussian Threshold
          ip::open(rect3x3Kernel, 2),    // Morph Open
          ip::close(rect3x3Kernel, 1)    // Morph Close
      });

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto descriptors = cd::filter(
      cd::find(preProcessedImage),
      {
          cd::removeIf(cd::areaSmallerThan(minimalSize)),
          cd::convexHull(),
          cd::approximateShape(cd::perimeterTimes(0.05)),
          cd::removeIf([](auto const &d)
                       { return d.contour.size() != 4; }),
          cd::removeIf([](auto const &d)
                       { return cd::maximumSideLengthRatio(d.contour) < (2. / 3.); }),
          cd::sortBy([](auto const &a, auto const &b)
                     { return cv::contourArea(a.contour) < cv::contourArea(b.contour); }),
          cd::annotateWith([](int index, auto &d)
                           { return std::make_tuple(
                                 "#" + std::to_string(index + 1),
                                 std::vector<std::string>{
                                     "area: " + std::to_string(cv::contourArea(d.contour))}); }),
          // cd::printTo(std::cout),
      });

  return DetectionResult{std::move(preProcessedImage), std::move(descriptors)};
}
