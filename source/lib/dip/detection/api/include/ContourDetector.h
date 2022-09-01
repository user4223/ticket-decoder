#pragma once

#include "ContourDetectorResult.h"

#include <opencv2/core.hpp>

class ContourDetector
{
public:
  virtual ~ContourDetector() = default;

  virtual ContourDetectorResult detect(cv::Mat const &image) = 0;
};
