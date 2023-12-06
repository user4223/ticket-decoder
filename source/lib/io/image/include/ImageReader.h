#pragma once

#include "lib/io/api/include/Reader.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::image
{
    class ImageReader : public io::api::Reader
    {
        ::utility::Logger logger;

    public:
        ImageReader(::utility::LoggerFactory &loggerFactory);

        std::vector<std::string> supportedExtensions() const override;

        api::ReadResult read(std::filesystem::path path) const override;

        api::ReadResult read(std::filesystem::path path, api::ReadOptions options) const override;
    };
}
