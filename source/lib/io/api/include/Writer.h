#pragma once

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <string>

namespace io::api
{
    class SinkManager;

    class Writer
    {
        std::filesystem::path destination;

        Writer(std::filesystem::path destination);

        std::filesystem::path deriveOutputPath(std::string postfix, std::string extension);

    public:
        friend SinkManager;
        Writer(Writer const &) = delete;
        Writer(Writer &&) = default;
        Writer &operator=(Writer const &) = delete;
        Writer &operator=(Writer &&) = default;

        std::filesystem::path write(cv::Mat const &image, std::string postfix = "");

        std::filesystem::path write(std::vector<std::uint8_t> const &bytes, std::string postfix = "");

        std::filesystem::path write(std::string const &json, std::string postfix = "");
    };
}
