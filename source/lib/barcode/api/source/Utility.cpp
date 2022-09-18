
#include "../include/Utility.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

namespace barcode::api
{
  void dump(std::filesystem::path const &basePath, Result const &result, cv::Mat const &source)
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
