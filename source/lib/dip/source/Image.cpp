#include "../include/Image.h"

#include "lib/dip/include/Transform.h"

#include <opencv2/imgproc.hpp>

namespace dip
{
  void copyTo(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box)
  {
    if (source.empty() || box == cv::Rect{})
    {
      return;
    }

    auto const &part = source.channels() == 3 ? source : toColor(source.clone());
    part(cv::Rect(0, 0, box.width, box.height)).copyTo(destination(box));
  }

  std::vector<std::tuple<cv::Point, std::string>> getDimensionAnnotations(cv::Mat &image)
  {
    auto const cols = image.cols;
    auto const rows = image.rows;

    return {
        {cv::Point(0, 25), "0x0"},
        {cv::Point(cols - 125, 25), std::to_string(cols) + "x0"},
        {cv::Point(cols - 180, rows - 10), std::to_string(cols) + "x" + std::to_string(rows)},
        {cv::Point(0, rows - 10), "0x" + std::to_string(rows)}};
  }
}
