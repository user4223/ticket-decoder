
#include <opencv2/core.hpp>

#include <string>
#include <vector>

class Visualizer
{
  std::string const name;

public:
  Visualizer(std::string const &name = "Screen");

  void visualize(cv::Mat &input);

  void visualize(cv::Mat &input, std::vector<std::vector<cv::Point>> const &contours);
};
