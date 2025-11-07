
#include "../include/ForwardDetector.h"

#include "lib/detector/api/include/Descriptor.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "lib/dip/filtering/include/Transform.h"

namespace dip::detection::api
{
    ForwardDetector::ForwardDetector(infrastructure::Context &context, DetectorOptions o)
        : logger(CREATE_LOGGER(context.getLoggerFactory())), options(std::move(o))
    {
    }

    std::string ForwardDetector::getName() const { return "Forward"; }

    DetectorType ForwardDetector::getType() const { return DetectorType::NOP_FORWARDER; }

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
