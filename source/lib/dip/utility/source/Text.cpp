
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

  void drawBlueText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    putText(image, text, position, blue);
  }

  void drawBlueText(cv::Mat &image, std::vector<std::tuple<cv::Point, std::string>> const &pointTextTuples)
  {
    std::for_each(pointTextTuples.cbegin(), pointTextTuples.cend(),
                  [&](auto const &pointTextTuple)
                  { utility::drawBlueText(image, std::get<0>(pointTextTuple), std::get<1>(pointTextTuple)); });
  }

  void drawRedText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    putText(image, text, position, red);
  }

  void drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines)
  {
    auto offset = position.x;
    std::for_each(lines.begin(), lines.end(), [&](auto const &line)
                  { drawRedText(image, cv::Point(position.x, position.y + (offset += lineOffset)), line); });
  }

  void drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines)
  {
    drawRedText(image, position, lineOffset, splitLines(lines));
  }

  void drawBlueDimensions(cv::Mat &image)
  {
    auto const cols = image.cols;
    auto const rows = image.rows;

    drawBlueText(image, cv::Point(0, 25), "0x0");
    drawBlueText(image, cv::Point(cols - 125, 25), std::to_string(cols) + "x0");
    drawBlueText(image, cv::Point(cols - 180, rows - 10), std::to_string(cols) + "x" + std::to_string(rows));
    drawBlueText(image, cv::Point(0, rows - 10), "0x" + std::to_string(rows));
  }

}
