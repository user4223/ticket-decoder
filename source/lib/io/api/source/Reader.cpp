#include "../include/Reader.h"

#include "../../image/include/ImageReader.h"
#include "../../pdf/include/PdfReader.h"

#include <algorithm>

namespace io::api
{
    ReaderOptions const ReaderOptions::DEFAULT = ReaderOptions{};

    std::vector<std::shared_ptr<Reader>> Reader::create(::utility::LoggerFactory &loggerFactory, ReaderOptions options)
    {
        return {
            std::shared_ptr<Reader>(new image::ImageReader(loggerFactory, options)),
            std::shared_ptr<Reader>(new pdf::PdfReader(loggerFactory, options))};
    }

    std::string Reader::normalizeExtension(std::filesystem::path const &path)
    {
        auto extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return extension;
    }

    void Reader::validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("Requested input file does not exist: " + path.string());
        }
        if (!std::filesystem::is_regular_file(path))
        {
            throw std::runtime_error("Requested input file is not a regular file: " + path.string());
        }

        auto extension = normalizeExtension(path);
        if (std::find(allowedLowerCaseExtensions.cbegin(), allowedLowerCaseExtensions.cend(), extension) == allowedLowerCaseExtensions.cend())
        {
            throw std::runtime_error("Requested input file has unexpected extension: " + path.string());
        }
    }
}
