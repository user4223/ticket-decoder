#pragma once

#include <opencv2/core.hpp>

#include <string>
#include <vector>
#include <utility>

namespace dip
{

  unsigned int drawBlueText(cv::Mat &image, cv::Point const &position, std::string text);

  unsigned int drawBlueText(cv::Mat &image, std::vector<std::tuple<cv::Point, std::string>> const &pointTextTuples);

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, std::string text);

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines);

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, int columnOffset, std::vector<std::pair<std::string, std::string>> lines);

  unsigned int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines);

} // namespace dip::utility
