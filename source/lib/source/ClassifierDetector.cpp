
#include "../dip/filtering/include/Transform.h"

#include "../include/ClassifierDetector.h"
#include "../include/ContourDetectorFilters.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here
#include <filesystem>

struct ClassifierDetector::Internal
{
  std::unique_ptr<cv::CascadeClassifier> classifier;

  Internal()
  {
    // std::string const file = "etc/aztec-classifier.xml";
    std::string const file = "etc/haarcascade_frontalface_default.xml";
    if (!std::filesystem::exists(file))
    {
      throw std::domain_error("Required classifier file not found: " + file);
    }
    classifier = std::make_unique<cv::CascadeClassifier>(file);
  }
};

ClassifierDetector::ClassifierDetector()
    : internal(std::make_shared<Internal>()) {}

std::unique_ptr<ContourDetector> ClassifierDetector::create()
{
  return std::unique_ptr<ContourDetector>{new ClassifierDetector()};
}

ContourDetectorResult ClassifierDetector::detect(cv::Mat const &input)
{
  using cd = ContourDetectorFilters;

  auto preProcessedImage = dip::filtering::toGray(input);
  auto objects = std::vector<cv::Rect>{};
  internal->classifier->detectMultiScale(preProcessedImage, objects);

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto descriptor = cd::filter(
      ContourSetDescriptor::fromContours(ContourDescriptor::fromRects(std::move(objects))),
      {
          cd::removeIf(cd::areaSmallerThan(minimalSize)), //
          cd::sortBy(cd::biggestArea()),                  //
          cd::removeIfParent(),                           //
          cd::annotateWith({cd::dimensionString()}),
          /* cd::printTo(std::cout) */
      });

  return ContourDetectorResult{std::move(descriptor.contours)};
}
