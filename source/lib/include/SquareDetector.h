#pragma once

#include "Detector.h"
#include "ImageProcessor.h"
#include "ContourDetector.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

class SquareDetector : public Detector
{
private:
  ImageProcessor const &imageProcessor;
  ContourDetector const &contourDetector;

  SquareDetector(ImageProcessor const &imageProcessor, ContourDetector const &contourDetector);

public:
  static std::unique_ptr<Detector> create(ImageProcessor const &imageProcessor, ContourDetector const &contourDetector);

  DetectionResult detect(cv::Mat const &image);
};