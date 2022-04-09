
#include "../include/ClassifierDetector.h"
#include "../include/ImageProcessor.h"

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

  auto preProcessedImage = ip::toGray(input);
  auto result = DetectionResult{std::move(preProcessedImage)};
  internal->classifier->detectMultiScale(result.input, result.objects);
  return result;
}
