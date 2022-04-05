#pragma once

#include <opencv2/core.hpp>

#include "DetectionResult.h"

class Detector
{
public:
  virtual ~Detector() = default;

  virtual DetectionResult detect(cv::Mat const &image) = 0;
};