#pragma once

#include "lib/input/detail/api/include/Reader.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::image
{
    class ImageReader : public io::api::Reader
    {
        ::utility::Logger logger;
        api::LoadOptions options;

    public:
        ImageReader(infrastructure::Context &context, api::LoadOptions options);

        std::vector<std::string> supportedExtensions() const override;

        api::ReadResult read(std::filesystem::path path) const override;
    };
}
