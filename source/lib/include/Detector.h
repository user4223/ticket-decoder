
#include <opencv2/core/mat.hpp>

class Detector
{

  class Result
  {
    cv::Mat image;
  };

public:
  Result detect(cv::Mat image);
};