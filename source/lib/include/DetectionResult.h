
#include <opencv2/core.hpp>

#include <vector>

struct DetectionResult
{
  cv::Mat const &input;
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Rect> objects;

  DetectionResult(cv::Mat const &input);

  cv::Mat visualize(cv::Mat &destination);
};