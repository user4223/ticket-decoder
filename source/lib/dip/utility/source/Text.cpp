
#include "../include/Text.h"
#include "../include/Color.h"

#include <opencv2/imgproc.hpp>

#include <numeric>

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
    cv::putText(image, text, position, cv::FONT_HERSHEY_DUPLEX, 1., color, 2);
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

  int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines)
  {
    auto offset = position.x;
    return std::accumulate(lines.begin(), lines.end(), 0, [&](int const count, auto const &line)
                           {
                    auto const y = position.y + (offset += lineOffset);
                    if (image.rows < y)
                    {
                      return count;
                    }
                    drawRedText(image, cv::Point(position.x, y), line);
                    return count + 1; });
  }

  int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, int columnOffset, std::vector<std::pair<std::string, std::string>> lines)
  {
    auto offset = position.x;
    return std::accumulate(lines.begin(), lines.end(), 0, [&](int const count, auto const &line)
                           {
                    auto const lo = offset += lineOffset;
                    auto const y = position.y + lo;
                    if (image.rows < y)
                    {
                      return count;
                    }
                    drawRedText(image, cv::Point(position.x, y), line.first);
                    drawRedText(image, cv::Point(position.x + columnOffset, y), line.second);
                    return count + 1; });
  }

  void drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines)
  {
    drawRedText(image, position, lineOffset, splitLines(lines));
  }
}
