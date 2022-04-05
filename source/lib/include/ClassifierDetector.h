#pragma once

#include <opencv2/core.hpp>

#include <memory>

#include "Detector.h"
#include "ImageProcessor.h"

class ClassifierDetector : public Detector
{
  ImageProcessor const &imageProcessor;
  struct Internal;
  std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

public:
  ClassifierDetector(ImageProcessor const &imageProcessor);

  DetectionResult detect(cv::Mat const &image);
};