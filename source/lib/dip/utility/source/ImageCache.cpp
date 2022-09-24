
#include "../include/ImageCache.h"

#include <opencv2/imgcodecs.hpp>

#include <map>

namespace dip::utility
{
  static auto imageCache = std::map<std::filesystem::path, cv::Mat>{};

  cv::Mat getImage(std::filesystem::path const &filePath)
  {
    auto const cacheEntry = imageCache.find(filePath);
    if (cacheEntry == imageCache.end())
    {
      auto const entry = imageCache.insert(std::make_tuple(filePath, cv::imread(filePath.string())));
      return entry.first->second.clone();
    }

    return cacheEntry->second.clone();
  }
}
