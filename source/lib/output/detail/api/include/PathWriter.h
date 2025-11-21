#pragma once

#include "Writer.h"

namespace output::detail
{
    class PathWriter : public Writer
    {
        std::filesystem::path destination;
        bool suppressPathExpansion;

        std::filesystem::path deriveOutputPath(std::string postfix, std::string extension);

    public:
        PathWriter(std::filesystem::path destination, bool suppressPathExpansion = false);
        PathWriter(PathWriter const &) = delete;
        PathWriter(PathWriter &&) = default;
        PathWriter &operator=(PathWriter const &) = delete;
        PathWriter &operator=(PathWriter &&) = default;

        std::filesystem::path write(cv::Mat const &image, std::string postfix = "") override;

        std::filesystem::path write(std::vector<std::uint8_t> const &bytes, std::string postfix = "") override;

        std::filesystem::path write(std::string const &json, std::string postfix = "") override;
    };
}
