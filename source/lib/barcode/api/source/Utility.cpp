
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
  static auto const red = cv::Scalar(0, 0, 255);

  struct Property
  {
    cv::Scalar color;
    int thickness;
  };

  static const std::map<Level, Property> propertyMap = {
      {Level::Unknown, Property{red, 1}},
      {Level::Detected, Property{yellow, 2}},
      {Level::Decoded, Property{green, 3}}};
  static const std::map<Level, char> characterMap = {
      {Level::Unknown, '-'},
      {Level::Detected, '.'},
      {Level::Decoded, '+'}};

  Property getPropery(Level level)
  {
    auto const entry = propertyMap.find(level);
    return entry == propertyMap.end() ? Property{red, 1} : entry->second;
  }

  char getCharacter(Level level)
  {
    auto const charIterator = characterMap.find(level);
    return charIterator == characterMap.end() ? ' ' : charIterator->second;
  }

  void visualize(cv::Mat &input, Result const &result)
  {
    input = dip::filtering::toColor(std::move(input));
    auto const property = getPropery(result.level);
    cv::rectangle(input, result.box.tl(), result.box.br(), property.color, property.thickness);
  }

  void visualize(std::ostream &stream, Result const &result)
  {
    stream << getCharacter(result.level) << std::flush;
  }
}
