
#include "../include/Result.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <map>
#include <ostream>

namespace barcode::api
{
  static auto const yellow = cv::Scalar(0, 255, 255);
  static auto const green = cv::Scalar(0, 255, 0);
  static auto const blue = cv::Scalar(255, 0, 0);

  static std::map<Level, cv::Scalar> colorMap = {
      {Level::Detected, yellow},
      {Level::Decoded, green}};

  static cv::Scalar getColor(Level level)
  {
    auto const colorIterator = colorMap.find(level);
    return colorIterator == colorMap.end() ? cv::Scalar(0, 0, 255) : colorIterator->second;
  }

  Result::Result(unsigned int id, cv::Rect const &box) : id(id), box(box) {}

  bool Result::isDecoded() const
  {
    return level == barcode::api::Level::Decoded && !payload.empty();
  }

  cv::Mat Result::visualize(cv::Mat &&input) const
  {
    if (level == Level::Unknown)
    {
      return std::move(input);
    }

    auto destination = input.channels() == 3 ? std::move(input) : [input = std::move(input)]()
    {
      cv::Mat transformed;
      cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
      return transformed;
    }();

    auto const color = getColor(level);
    cv::rectangle(destination, box.tl(), box.br(), color, 2);

    // std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
    //               {
    //                                   auto const [position, text] = annotator(d);
    //                                   cv::putText(destination, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness); });

    return destination;
  }

  static std::map<Level, std::string> decodingResultMap = {
      {Level::Detected, "."},
      {Level::Decoded, "+"}};

  Result Result::visualize(Result &&result, std::ostream &stream)
  {
    auto output = decodingResultMap.find(result.level);
    if (output != decodingResultMap.end())
    {
      stream << output->second << std::flush;
    }
    return std::move(result);
  }
}
