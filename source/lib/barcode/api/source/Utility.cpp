
#include "../include/Utility.h"
#include "lib/dip/filtering/include/Transform.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <fstream>
#include <map>

namespace barcode::api
{
  void dump(std::filesystem::path const &basePath, Result const &result)
  {
    if (!std::filesystem::exists(basePath.parent_path()))
    {
      std::filesystem::create_directories(basePath.parent_path());
    }
    auto const outputPath = basePath.string();
    if (result.isDecoded())
    {
      std::ofstream{outputPath + ".raw", std::ios::binary}.write(
          (char const *)&(result.payload[0]),
          result.payload.size());
    }

    if (!result.image.empty())
    {
      auto imageOutputPath = outputPath;
      switch (result.level)
      {
      case barcode::api::Level::Decoded:
        imageOutputPath += "_decoded";
        break;
      case barcode::api::Level::Detected:
        imageOutputPath += "_detected";
        break;
      default:
        imageOutputPath += "_failed";
        break;
      }
      cv::imwrite(imageOutputPath + ".jpg", result.image);
    }
  }

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

  void visualize(cv::Mat &input, Result const &result)
  {
    if (result.level == Level::Unknown)
    {
      return;
    }

    input = dip::filtering::toColor(std::move(input));

    auto const color = getColor(result.level);
    cv::rectangle(input, result.box.tl(), result.box.br(), color, 2);

    // std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
    //               {
    //                                   auto const [position, text] = annotator(d);
    //                                   cv::putText(destination, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., cyan, coordinateThickness); });
  }

  static std::map<Level, std::string> decodingResultMap = {
      {Level::Detected, "."},
      {Level::Decoded, "+"}};

  void visualize(std::ostream &stream, Result const &result)
  {
    auto output = decodingResultMap.find(result.level);
    if (output != decodingResultMap.end())
    {
      stream << output->second << std::flush;
    }
  }
}
