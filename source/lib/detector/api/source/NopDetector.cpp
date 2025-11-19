
#include "../include/NopDetector.h"

#include "lib/detector/api/include/Descriptor.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "lib/dip/include/Transform.h"

namespace detector::api
{
    NopDetector::NopDetector(infrastructure::Context &context, DetectorOptions o)
        : options(std::move(o))
    {
    }

    std::string NopDetector::getName() const { return "Forward"; }

    DetectorType NopDetector::getType() const { return DetectorType::NOP_DETECTOR; }

    Result NopDetector::detect(cv::Mat const &input)
    {
        auto gray = dip::filtering::toGray(input);
        auto const size = gray.size();
        return Result{
            {api::Descriptor{0, {}, cv::Rect{2, 2, size.width - 4, size.height - 4}, std::move(gray)}},
            std::nullopt,
            std::nullopt};
    }
}
