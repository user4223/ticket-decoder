#pragma once

#include "Detector.h"
#include "ImageProcessor.h"
#include "ContourDetector.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

class SquareDetector : public Detector
{
public:
  static std::unique_ptr<Detector> create();

  DetectionResult detect(cv::Mat const &image);
};