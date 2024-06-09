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
        api::ReaderOptions options;

    public:
        ImageReader(::utility::LoggerFactory &loggerFactory, api::ReaderOptions options);

        std::vector<std::string> supportedExtensions() const override;

        api::ReadResult read(std::filesystem::path path) const override;
    };
}
