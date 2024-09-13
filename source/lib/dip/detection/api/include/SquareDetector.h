#pragma once

#include "DetectorOptions.h"
#include "Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/DebugController.h"
#include "lib/utility/include/Logger.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace dip::detection::api
{
  class SquareDetector : public Detector
  {
    ::utility::Logger logger;
    ::utility::DebugController &debugController;
    DetectorOptions options;

  public:
    SquareDetector(infrastructure::Context &context, DetectorOptions options);

    bool isOperational() const override { return true; }

    Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    DetectorType getType() const override;
  };
}
