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
        return {".png", ".jpeg", ".jpg"};
    }

    api::ReadResult ImageReader::read(std::filesystem::path path) const
    {
        Reader::validate(path, supportedExtensions());
        LOG_DEBUG(logger) << "Reading input: " << path;
        return api::ReadResult(cv::imread(path.string()));
    }
}
