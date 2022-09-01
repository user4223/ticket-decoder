#pragma once

#include "ContourDetector.h"

#include <opencv2/core.hpp>

#include <memory>

class ClassifierDetector : public ContourDetector
{
  struct Internal;
  std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

  ClassifierDetector();

public:
  static std::unique_ptr<ContourDetector> create();

  ContourDetectorResult detect(cv::Mat const &image) override;
};