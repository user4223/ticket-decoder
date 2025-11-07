#pragma once

#include "lib/detector/api/include/Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace dip::detection::api
{
  class ForwardDetector : public Detector
  {
    ::utility::Logger logger;
    DetectorOptions options;

  public:
    ForwardDetector(infrastructure::Context &context, DetectorOptions options);

    bool isOperational() const override { return true; }

    Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    DetectorType getType() const override;
  };
}
