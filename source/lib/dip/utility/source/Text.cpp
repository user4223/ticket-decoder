
#include "../include/Text.h"

#include <opencv2/imgproc.hpp>

namespace dip::utility
{
  static auto const blue = cv::Scalar(255, 0, 0);
  static auto const red = cv::Scalar(0, 0, 255);
  static auto const yellow = cv::Scalar(0, 255, 255);

  void putText(cv::Mat &image, std::string text, cv::Point const &position, cv::Scalar const &color)
  {
    cv::putText(image, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., color, 2);
  }

  void putBlueText(cv::Mat &image, std::string text, cv::Point const &position)
  {
    putText(image, text, position, blue);
  }

  void putRedText(cv::Mat &image, std::string text, cv::Point const &position)
  {
    putText(image, text, position, red);
  }

  void putBlueDimensions(cv::Mat &image)
  {
    auto const cols = image.cols;
    auto const rows = image.rows;

    putBlueText(image, "0x0", cv::Point(0, 25));
    putBlueText(image, std::to_string(cols) + "x0", cv::Point(cols - 125, 25));
    putBlueText(image, std::to_string(cols) + "x" + std::to_string(rows), cv::Point(cols - 180, rows - 10));
    putBlueText(image, "0x" + std::to_string(rows), cv::Point(0, rows - 10));
  }

}
