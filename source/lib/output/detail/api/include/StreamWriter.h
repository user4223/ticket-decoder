#pragma once

#include "Writer.h"

namespace output::detail
{
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
