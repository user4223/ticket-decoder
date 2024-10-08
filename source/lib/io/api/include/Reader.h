#pragma once

#include "ReadResult.h"

#include "lib/infrastructure/include/ContextFwd.h"

#include <filesystem>
#include <vector>
#include <memory>

namespace io::api
{
    struct ReaderOptions
    {
        unsigned int dpi = 300;
        std::vector<unsigned int> pageIndexes = {};
        bool grayscale = true;

        static ReaderOptions const DEFAULT;
    };

    class Reader
    {
    public:
        virtual ~Reader() = default;

        virtual std::vector<std::string> supportedExtensions() const = 0;

        virtual ReadResult read(std::filesystem::path path) const = 0;

        static std::vector<std::shared_ptr<Reader>> create(infrastructure::Context &context, ReaderOptions options = {});

        static void validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions);
    };
}
