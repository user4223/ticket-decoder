#pragma once

#include "Detector.h"
#include "ImageProcessor.h"
#include "ContourDetector.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

class SquareDetector : public Detector
{
  Parameters &parameters;

  SquareDetector(Parameters &parameters);

public:
  static std::unique_ptr<Detector> create(Parameters &parameters);

  DetectionResult detect(cv::Mat const &image);
};