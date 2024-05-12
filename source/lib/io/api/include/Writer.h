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

    public:
        friend SinkManager;

        std::filesystem::path write(cv::Mat const &image);

        std::filesystem::path write(std::vector<std::uint8_t> const &bytes);

        std::filesystem::path write(std::string const &json);
    };
}
