
#include <opencv2/core.hpp>

#include "Detector.h"

class ContourDetector : public Detector
{
public:
  DetectionResult detect(cv::Mat const &image);
};