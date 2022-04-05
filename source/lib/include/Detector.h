
#include <opencv2/core.hpp>

#include <vector>
#include <memory>

#include "DetectionResult.h"

class Detector
{
  struct Internal;
  std::shared_ptr<Internal> internal; // by intention a shared_ptr 2 allow forward declared type here

public:
  Detector();

  DetectionResult detect(cv::Mat const &image);
};