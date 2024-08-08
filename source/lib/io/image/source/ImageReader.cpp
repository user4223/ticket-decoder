#include "../include/ImageReader.h"

#include "lib/utility/include/Logging.h"

#include <opencv2/imgcodecs.hpp>

namespace io::image
{
    ImageReader::ImageReader(::utility::LoggerFactory &loggerFactory, api::ReaderOptions o)
        : logger(CREATE_LOGGER(loggerFactory)), options(std::move(o))
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
        return api::ReadResult(cv::imread(path.string(), options.grayscale
                                                             ? cv::ImreadModes::IMREAD_GRAYSCALE
                                                             : cv::ImreadModes::IMREAD_COLOR));
    }
}
