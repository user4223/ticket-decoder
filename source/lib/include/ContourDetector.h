#pragma once

#include <opencv2/core.hpp>

#include "Detector.h"
#include "ImageProcessor.h"

class ContourDetector : public Detector
{
private:
  ImageProcessor const &imageProcessor;

public:
  ContourDetector(ImageProcessor const &ImageProcessor);

  DetectionResult detect(cv::Mat const &image);
};