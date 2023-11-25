#pragma once

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::api
{
    class Scanner
    {
    public:
        virtual ~Scanner() = default;

        virtual std::vector<cv::Mat> scan(std::filesystem::path path) const = 0;
    };
}
