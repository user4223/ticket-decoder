
#include "../include/Text.h"
#include "../include/Color.h"

#include <opencv2/imgproc.hpp>

namespace dip::utility
{
  std::vector<std::string> splitLines(std::string const &lines)
  {
    auto result = std::vector<std::string>{};
    auto stream = std::stringstream{lines};

    for (std::string line; std::getline(stream, line, '\n');)
    {
      result.push_back(line);
    }
    return result;
  }

  void putText(cv::Mat &image, std::string text, cv::Point const &position, cv::Scalar const &color)
  {
    cv::putText(image, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., color, 2);
  }

  void putBlueText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    putText(image, text, position, blue);
  }

  void putBlueText(cv::Mat &image, std::vector<std::tuple<cv::Point, std::string>> const &pointTextTuples)
  {
    std::for_each(pointTextTuples.cbegin(), pointTextTuples.cend(),
                  [&](auto const &pointTextTuple)
                  { utility::putBlueText(image, std::get<0>(pointTextTuple), std::get<1>(pointTextTuple)); });
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

  void putRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines)
  {
    putRedText(image, position, lineOffset, splitLines(lines));
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
