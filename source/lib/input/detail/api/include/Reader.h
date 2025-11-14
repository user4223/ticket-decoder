#pragma once

#include "ReadResult.h"

#include "lib/input/api/include/LoadOptions.h"

#include "lib/infrastructure/include/ContextFwd.h"

#include <filesystem>
#include <vector>
#include <memory>

namespace io::api
{
    class Reader
    {
    public:
        virtual ~Reader() = default;

        virtual std::vector<std::string> supportedExtensions() const = 0;

        virtual ReadResult read(std::filesystem::path path) const = 0;

        static std::vector<std::shared_ptr<Reader>> createAll(infrastructure::Context &context, LoadOptions options = {});

        static void validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions);
    };
}
