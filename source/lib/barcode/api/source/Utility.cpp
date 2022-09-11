
#include "../include/Utility.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

namespace barcode::api
{
  void dump(std::filesystem::path const &basePath, Result const &result, cv::Mat const &source)
  {
    auto const outputPath = basePath.string();
    if (result.level == barcode::api::Level::Decoded && !result.payload.empty())
    {
      std::ofstream{outputPath + ".raw", std::ios::binary}.write(
          (char const *)&(result.payload[0]),
          result.payload.size());
    }

    if (!source.empty())
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
      cv::imwrite(imageOutputPath + ".jpg", source);
    }
  }
}
