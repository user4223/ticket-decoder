
#include <opencv2/core.hpp>

#include <vector>
#include <memory>

class Detector
{

  struct Result
  {
    cv::Mat const &input;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Rect> objects;

    Result(cv::Mat const &input);
    Result(cv::Mat const &input, std::vector<std::vector<cv::Point>> &&contours);
    Result(cv::Mat const &input, std::vector<cv::Rect> &&objects);

    cv::Mat visualize(cv::Mat &destination);
  };

  struct Internal;
  std::shared_ptr<Internal> internal;

public:
  Detector();

  Result detect(cv::Mat const &image);
};