#pragma once

#include "ContourDetectorParameters.h"
#include "ContourDetector.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

class SquareDetector : public ContourDetector
{
  ContourDetectorParameters &parameters;

  SquareDetector(ContourDetectorParameters &parameters);

public:
  static std::unique_ptr<ContourDetector> create(ContourDetectorParameters &parameters);

  ContourDetectorResult detect(cv::Mat const &image) override;
};