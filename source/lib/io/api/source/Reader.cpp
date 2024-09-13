#include "../include/Reader.h"
#include "../include/Utility.h"

#include "../../image/include/ImageReader.h"
#include "../../pdf/include/PdfReader.h"

namespace io::api
{
    ReaderOptions const ReaderOptions::DEFAULT = ReaderOptions{};

    std::vector<std::shared_ptr<Reader>> Reader::create(infrastructure::Context &context, ReaderOptions options)
    {
        return {
            std::shared_ptr<Reader>(new image::ImageReader(context, options)),
            std::shared_ptr<Reader>(new pdf::PdfReader(context, options))};
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

        auto const extension = utility::normalizeExtension(path);
        if (std::find(allowedLowerCaseExtensions.cbegin(), allowedLowerCaseExtensions.cend(), extension) == allowedLowerCaseExtensions.cend())
        {
            throw std::runtime_error("Requested input file has unexpected extension: " + path.string());
        }
    }
}
