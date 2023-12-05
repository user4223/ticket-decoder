#pragma once

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>

namespace io::api
{
    class Scanner
    {
    public:
        virtual ~Scanner() = default;

        virtual std::vector<cv::Mat> scan(std::filesystem::path path) const = 0;

        static std::vector<std::filesystem::path> scan(std::filesystem::path directory, std::vector<std::string> extensions);
    };
}
