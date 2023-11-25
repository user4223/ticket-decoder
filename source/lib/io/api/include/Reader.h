#pragma once

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::api
{
    class Reader
    {
    public:
        virtual ~Reader() = default;

        virtual std::vector<std::string> supportedFileExtensions() const = 0;

        virtual cv::Mat read(std::filesystem::path path) const = 0;

        static void validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions);
    };
}
