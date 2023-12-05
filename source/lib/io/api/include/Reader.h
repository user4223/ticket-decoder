#pragma once

#include "ReadResult.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::api
{
    class Reader
    {
    public:
        virtual ~Reader() = default;

        virtual std::vector<std::string> supportedExtensions() const = 0;

        virtual ReadResult read(std::filesystem::path path) const = 0;

        virtual ReadResult read(std::filesystem::path path, std::vector<unsigned int> pages) const = 0;

        static void validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions);
    };
}
