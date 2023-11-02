#pragma once

#include <filesystem>

#include <opencv2/core.hpp>

namespace dip::utility
{

    cv::Mat read(std::filesystem::path const& path);

}
