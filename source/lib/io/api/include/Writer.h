#pragma once

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <string>

namespace io::api
{
    class Writer
    {
    public:
        virtual ~Writer() = default;

        virtual std::filesystem::path write(cv::Mat const &image, std::string postfix = "") = 0;

        virtual std::filesystem::path write(std::vector<std::uint8_t> const &bytes, std::string postfix = "") = 0;

        virtual std::filesystem::path write(std::string const &json, std::string postfix = "") = 0;
    };

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

    class StreamWriter : public Writer
    {
        struct Internal;
        std::shared_ptr<Internal> internal;

    public:
        StreamWriter(std::ostream &stream, std::string annotation);
        StreamWriter(StreamWriter const &) = delete;
        StreamWriter(StreamWriter &&) = default;
        StreamWriter &operator=(StreamWriter const &) = delete;
        StreamWriter &operator=(StreamWriter &&) = default;

        std::filesystem::path write(cv::Mat const &image, std::string postfix = "") override;

        std::filesystem::path write(std::vector<std::uint8_t> const &bytes, std::string postfix = "") override;

        std::filesystem::path write(std::string const &json, std::string postfix = "") override;
    };
}
