#pragma once

#include <opencv2/core.hpp>

#include <string>
#include <vector>
#include <utility>

namespace dip::utility
{

  void drawBlueText(cv::Mat &image, cv::Point const &position, std::string text);

  void drawBlueText(cv::Mat &image, std::vector<std::tuple<cv::Point, std::string>> const &pointTextTuples);

  void drawRedText(cv::Mat &image, cv::Point const &position, std::string text);

  int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::vector<std::string> lines);

  int drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, int columnOffset, std::vector<std::pair<std::string, std::string>> lines);

  void drawRedText(cv::Mat &image, cv::Point const &position, int lineOffset, std::string lines);

} // namespace dip::utility
