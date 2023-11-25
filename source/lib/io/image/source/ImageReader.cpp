#include "../include/ImageReader.h"

#include "lib/utility/include/Logging.h"

#include <opencv2/imgcodecs.hpp>

namespace io::image
{
    ImageReader::ImageReader(::utility::LoggerFactory &loggerFactory)
        : logger(CREATE_LOGGER(loggerFactory))
    {
    }

    std::vector<std::string> ImageReader::supportedFileExtensions() const
    {
        return {"png", "jpeg", "jpg"};
    }

    cv::Mat ImageReader::read(std::filesystem::path path) const
    {
        Reader::validate(path, supportedFileExtensions());
        LOG_DEBUG(logger) << "Reading input: " << path;
        return cv::imread(path.string());
    }
}
