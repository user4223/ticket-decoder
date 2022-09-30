
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

  void putBlueText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    putText(image, text, position, blue);
  }

  void putRedText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    putText(image, text, position, red);
  }

  void putRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines)
  {
    auto offset = position.x;
    std::for_each(lines.begin(), lines.end(), [&](auto const &line)
                  { putRedText(image, cv::Point(position.x, position.y + (offset += lineOffset)), line); });
  }

  void putBlueDimensions(cv::Mat &image)
  {
    auto const cols = image.cols;
    auto const rows = image.rows;

    putBlueText(image, cv::Point(0, 25), "0x0");
    putBlueText(image, cv::Point(cols - 125, 25), std::to_string(cols) + "x0");
    putBlueText(image, cv::Point(cols - 180, rows - 10), std::to_string(cols) + "x" + std::to_string(rows));
    putBlueText(image, cv::Point(0, rows - 10), "0x" + std::to_string(rows));
  }

}
