
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

  unsigned int putText(cv::Mat &image, std::string text, cv::Point const &position, cv::Scalar const &color)
  {
    cv::putText(image, text, position, cv::FONT_HERSHEY_DUPLEX, 1., color, 2);
    return std::count_if(std::begin(text), std::end(text), [](char const c)
                         { return c == '\n'; });
  }

  unsigned int drawBlueText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    return putText(image, text, position, blue);
  }

  unsigned int drawBlueText(cv::Mat &image, std::vector<std::tuple<cv::Point, std::string>> const &pointTextTuples)
  {
    return std::accumulate(pointTextTuples.cbegin(), pointTextTuples.cend(), 0u,
                           [&](auto counter, auto const &pointTextTuple)
                           { return counter + utility::drawBlueText(image, std::get<0>(pointTextTuple), std::get<1>(pointTextTuple)); });
  }

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, std::string text)
  {
    return putText(image, text, position, red);
  }

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines)
  {
    auto offset = position.x;
    return std::accumulate(lines.begin(), lines.end(), 0u, [&](unsigned int const count, auto const &line)
                           {
                    auto const y = position.y + (offset += lineOffset);
                    if (image.rows < y)
                    {
                      return count;
                    }
                    drawRedText(image, cv::Point(position.x, y), line);
                    return count + 1; });
  }

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, int columnOffset, std::vector<std::pair<std::string, std::string>> lines)
  {
    auto offset = position.x;
    return std::accumulate(lines.begin(), lines.end(), 0u, [&](unsigned int const count, auto const &line)
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

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines)
  {
    return drawRedText(image, position, lineOffset, splitLines(lines));
  }
}
