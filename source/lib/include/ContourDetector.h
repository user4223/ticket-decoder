#pragma once

#include "Detector.h"
#include "ImageProcessor.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

class ContourDetector : public Detector
{
private:
  ImageProcessor const &imageProcessor;

  ContourDetector(ImageProcessor const &imageProcessor);

public:
  using ContourType = std::vector<cv::Point>;

  static std::unique_ptr<Detector> create(ImageProcessor const &imageProcessor);

  DetectionResult detect(cv::Mat const &image);
};