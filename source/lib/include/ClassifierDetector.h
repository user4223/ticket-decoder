#pragma once

#include "Detector.h"
#include "ImageProcessor.h"

#include <opencv2/core.hpp>

#include <memory>

class ClassifierDetector : public Detector
{
  ImageProcessor const &imageProcessor;
  struct Internal;
  std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

  ClassifierDetector(ImageProcessor const &imageProcessor);

public:
  static std::unique_ptr<Detector> create(ImageProcessor const &imageProcessor);

  DetectionResult detect(cv::Mat const &image);
};