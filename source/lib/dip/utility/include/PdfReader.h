#pragma once

#include <filesystem>

#include <opencv2/core.hpp>

namespace dip::utility
{

    /* TODO This is just a stub right now, we have to fix conan toolchain refactoring before we can
       start with new huge dependencies like poppler and stuff.
     */
    cv::Mat read(std::filesystem::path const &path);

}
