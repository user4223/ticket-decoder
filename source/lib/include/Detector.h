#pragma once

#include <opencv2/core.hpp>

#include "DetectionResult.h"

#include <string>

class Detector
{
public:
  struct Parameters
  {
    unsigned int imageProcessingDebugStep = 0; // disabled
    unsigned int contourDetectorDebugStep = 0; // disabled

    std::string toString()
    {
      return "imageProcessingDebugStep: " + std::to_string(imageProcessingDebugStep) + " " + //
             "contourDetectorDebugStep: " + std::to_string(contourDetectorDebugStep);
    }
  };

  virtual ~Detector() = default;

  virtual DetectionResult detect(cv::Mat const &image) = 0;
};