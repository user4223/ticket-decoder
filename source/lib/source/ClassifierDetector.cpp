
#include "../include/ClassifierDetector.h"

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

ClassifierDetector::ClassifierDetector(ImageProcessor const &ip)
    : imageProcessor(ip), internal(std::make_shared<Internal>()) {}

DetectionResult ClassifierDetector::detect(cv::Mat const &input)
{
  auto result = DetectionResult{
      imageProcessor.process(input, {})};

  internal->classifier->detectMultiScale(input, result.objects);
  return result;
}
