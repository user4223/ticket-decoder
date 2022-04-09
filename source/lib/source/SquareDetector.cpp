
#include "../include/SquareDetector.h"
#include "../include/ContourDescriptor.h"

#include <opencv2/core.hpp> // Reduce include dependencies here

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

  auto preProcessedImage = ip::process(
      ip::toGray(input),
      {[](auto &&input)
       { return ip::equalize(std::move(input), *claheParameters); },
       [](auto &&input)
       { return ip::smooth(std::move(input), 7); },
       [](auto &&input)
       { return ip::binarize(std::move(input), 13, 1); },
       [](auto &&input)
       { return ip::open(std::move(input), rect3x3Kernel, 2); },
       [](auto &&input)
       { return ip::close(std::move(input), rect3x3Kernel, 1); }});

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto descriptors = cd::process(
      cd::find(preProcessedImage),
      {[&](auto &&descriptors)
       { return cd::removeIf(std::move(descriptors), [&](auto const &d)
                             { return cv::contourArea(d.contour) < minimalSize; }); },
       [](auto &&descriptors)
       { return cd::convexHull(std::move(descriptors)); },
       [](auto &&descriptors)
       { return cd::approximateShape(std::move(descriptors), [](auto const &d)
                                     { return 0.05 * cv::arcLength(d.contour, true); }); },
       [](auto &&descriptors)
       { return cd::removeIf(std::move(descriptors), [](auto const &d)
                             { return d.contour.size() != 4; }); },
       [](auto &&descriptors)
       { return cd::removeIf(std::move(descriptors), [](auto const &d)
                             { return cd::maximumSideLengthRatio(d.contour) < (2. / 3.); }); },
       [](auto &&descriptors)
       { return cd::sortBy(std::move(descriptors), [](auto const &a, auto const &b)
                           { return cv::contourArea(a.contour) < cv::contourArea(b.contour); }); },
       [](auto &&descriptors)
       { return cd::annotateWith(std::move(descriptors), [](int index, auto &d)
                                 { return std::make_tuple(
                                       "#" + std::to_string(index + 1),
                                       std::vector<std::string>{"area: " + std::to_string(cv::contourArea(d.contour))}); }); }});
  //[](auto &&descriptors)
  //{ return printTo(std::move(descriptors), std::cout); },
  //});

  return DetectionResult{std::move(preProcessedImage), std::move(descriptors)};
}
