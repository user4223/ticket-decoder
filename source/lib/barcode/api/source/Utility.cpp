
#include "../include/Utility.h"

#include "lib/dip/include/Color.h"

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

  static const std::map<Level, dip::utility::ShapeProperties> propertyMap =
      {
          {Level::Unknown, dip::utility::ShapeProperties{dip::utility::red, 1}},
          {Level::Detected, dip::utility::ShapeProperties{dip::utility::yellow, 2}},
          {Level::Decoded, dip::utility::ShapeProperties{dip::utility::green, 3}}};

  dip::utility::ShapeProperties getDrawProperties(Level level)
  {
    auto const entry = propertyMap.find(level);
    return entry == propertyMap.end() ? dip::utility::ShapeProperties{dip::utility::red, 1} : entry->second;
  }

  static const std::map<Level, char> characterMap =
      {
          {Level::Unknown, '-'},
          {Level::Detected, '.'},
          {Level::Decoded, '+'}};

  char getCharacter(Level level)
  {
    auto const charIterator = characterMap.find(level);
    return charIterator == characterMap.end() ? ' ' : charIterator->second;
  }
}
