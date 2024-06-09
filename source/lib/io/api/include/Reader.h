#pragma once

#include "ReadResult.h"

#include "lib/utility/include/LoggingFwd.h"

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

        static std::vector<std::shared_ptr<Reader>> create(::utility::LoggerFactory &loggerFactory, ReaderOptions options = {});

        static std::string normalizeExtension(std::filesystem::path const &path);

        static void validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions);
    };
}
