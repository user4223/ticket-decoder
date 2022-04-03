
#include <opencv2/core.hpp>

#include <vector>
#include <memory>

class Detector
{

  struct Result
  {
    cv::Mat const &input;
    std::vector<std::vector<cv::Point>> const contours;

    Result(cv::Mat const &input, std::vector<std::vector<cv::Point>> &&contours);
  };

  struct Internal;
  std::shared_ptr<Internal> internal;

public:
  Detector();

  Result detect(cv::Mat const &image);
};