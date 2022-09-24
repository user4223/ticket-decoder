#pragma once

#include <opencv2/core.hpp>

#include <filesystem>

namespace dip::utility
{
  cv::Mat getImage(std::filesystem::path const &filePath);
}
