#include "../include/ImageReader.h"

#include "lib/utility/include/Logging.h"

#include <opencv2/imgcodecs.hpp>

namespace io::image
{
    ImageReader::ImageReader(::utility::LoggerFactory &loggerFactory)
        : logger(CREATE_LOGGER(loggerFactory))
    {
    }

    std::vector<std::string> ImageReader::supportedExtensions() const
    {
        return {"png", "jpeg", "jpg"};
    }

    api::ReadResult ImageReader::read(std::filesystem::path path) const
    {
        Reader::validate(path, supportedExtensions());
        LOG_DEBUG(logger) << "Reading input: " << path;
        return api::ReadResult(cv::imread(path.string()));
    }

    api::ReadResult ImageReader::read(std::filesystem::path path, std::vector<unsigned int> pages) const
    {
        if (pages.empty() || pages.size() > 1 || pages[0] != 0)
        {
            throw std::runtime_error("Exactly one entry equal to 0 supported");
        }
        return read(path);
    }
}
