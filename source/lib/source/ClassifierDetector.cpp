
#include "../include/ClassifierDetector.h"
#include "../include/ImageProcessor.h"
#include "../include/ContourDetector.h"

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

std::unique_ptr<Detector> ClassifierDetector::create()
{
  return std::unique_ptr<Detector>{new ClassifierDetector()};
}

DetectionResult ClassifierDetector::detect(cv::Mat const &input)
{
  using ip = ImageProcessor;
  using cd = ContourDetector;

  auto preProcessedImage = ip::toGray(input);
  auto objects = std::vector<cv::Rect>{};
  internal->classifier->detectMultiScale(preProcessedImage, objects);

  auto const minimalSize = input.rows * input.cols * (1. / 150.);
  auto descriptors = cd::filter(
      ContourDescriptor::fromRects(std::move(objects)),
      {
          cd::removeIf(cd::areaSmallerThan(minimalSize)), //
          cd::sortBy(cd::biggestArea()),                  //
          cd::removeIfParent(),                           //
          cd::annotateWith([](auto &d)
                           { return std::vector<std::string>{
                                 "area: " + std::to_string((int)cv::contourArea(d.contour))}; }),
          /* cd::printTo(std::cout) */
      });

  return DetectionResult{std::move(preProcessedImage), std::move(descriptors)};
}
