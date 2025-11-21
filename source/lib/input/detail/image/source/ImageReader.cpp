#include "../include/ImageReader.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/imgcodecs.hpp>

namespace input::detail
{
    ImageReader::ImageReader(infrastructure::Context &context, api::LoadOptions o)
        : logger(CREATE_LOGGER(context.getLoggerFactory())), options(std::move(o))
    {
    }

    std::vector<std::string> ImageReader::supportedExtensions() const
    {
        return {".png", ".jpeg", ".jpg"};
    }

    ReadResult ImageReader::read(std::filesystem::path path) const
    {
        Reader::validate(path, supportedExtensions());
        LOG_DEBUG(logger) << "Reading input: " << path;
        return ReadResult(cv::imread(path.string(), options.grayscale
                                                        ? cv::ImreadModes::IMREAD_GRAYSCALE
                                                        : cv::ImreadModes::IMREAD_COLOR));
    }
}
