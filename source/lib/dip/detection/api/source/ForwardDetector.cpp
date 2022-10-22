
#include "../include/ForwardDetector.h"
#include "../include/Descriptor.h"

#include "../../dip/filtering/include/Transform.h"

namespace dip::detection::api
{
    ForwardDetector::ForwardDetector(Parameters &p) : parameters(p) {}

    std::unique_ptr<Detector> ForwardDetector::create(Parameters &parameters)
    {
        return std::unique_ptr<Detector>{new ForwardDetector(parameters)};
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
