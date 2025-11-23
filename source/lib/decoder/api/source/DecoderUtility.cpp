// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/DecoderUtility.h"

#include "lib/dip/include/Color.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <fstream>
#include <map>

namespace decoder::api
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
      case decoder::api::Level::Decoded:
        imageOutputPath += "_decoded";
        break;
      case decoder::api::Level::Detected:
        imageOutputPath += "_detected";
        break;
      default:
        imageOutputPath += "_failed";
        break;
      }
      cv::imwrite(imageOutputPath + ".jpg", result.image);
    }
  }

  static const std::map<Level, dip::ShapeProperties> propertyMap =
      {
          {Level::Unknown, dip::ShapeProperties{dip::red, 1}},
          {Level::Detected, dip::ShapeProperties{dip::yellow, 2}},
          {Level::Decoded, dip::ShapeProperties{dip::green, 3}}};

  dip::ShapeProperties getDrawProperties(Level level)
  {
    auto const entry = propertyMap.find(level);
    return entry == propertyMap.end() ? dip::ShapeProperties{dip::red, 1} : entry->second;
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
