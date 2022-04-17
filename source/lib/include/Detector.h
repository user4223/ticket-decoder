#pragma once

#include <opencv2/core.hpp>

#include "DetectionResult.h"

#include <string>

class Detector
{
public:
  struct Parameters
  {
    int a = 0;
    int b = 0;

    std::string toString()
    {
      return "a: " + std::to_string(a) + ", b: " + std::to_string(b);
    }
  };

  virtual ~Detector() = default;

  virtual DetectionResult detect(cv::Mat const &image) = 0;
};