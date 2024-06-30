
#include "../include/ForwardDetector.h"
#include "../include/Descriptor.h"

#include "lib/dip/filtering/include/Transform.h"
#include "lib/utility/include/Logging.h"

namespace dip::detection::api
{
    ForwardDetector::ForwardDetector(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorOptions o)
        : logger(CREATE_LOGGER(loggerFactory)), options(std::move(o))
    {
    }

    std::string ForwardDetector::getName() { return "Forward"; }

    Result ForwardDetector::detect(cv::Mat const &input)
    {
        auto gray = dip::filtering::toGray(input);
        auto const size = gray.size();
        return Result{
            {api::Descriptor{0, {}, cv::Rect{2, 2, size.width - 4, size.height - 4}, std::move(gray)}},
            std::nullopt,
            std::nullopt};
    }
}
