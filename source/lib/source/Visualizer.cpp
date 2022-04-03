
#include "../include/Visualizer.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

Visualizer::Visualizer(std::string const &n) : name(n)
{
  cv::namedWindow(name);
}

void Visualizer::visualize(cv::Mat &input)
{
  cv::imshow(name, input);
}

void Visualizer::visualize(cv::Mat &input, std::vector<std::vector<cv::Point>> const &contours)
{
  if (input.channels() == 3)
  {
    cv::drawContours(input, contours, -1, cv::Scalar(0, 0, 255));
    visualize(input);
    return;
  }

  cv::Mat transformed;
  cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
  cv::drawContours(transformed, contours, -1, cv::Scalar(0, 0, 255));
  visualize(transformed);
}