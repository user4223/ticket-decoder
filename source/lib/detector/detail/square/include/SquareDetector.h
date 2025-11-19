#pragma once

#include "lib/detector/api/include/Detector.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/DebugController.h"
#include "lib/utility/include/Logger.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace detector::detail
{
  class SquareDetector : public api::Detector
  {
    ::utility::Logger logger;
    ::utility::DebugController &debugController;
    api::DetectorOptions options;

  public:
    SquareDetector(infrastructure::Context &context, api::DetectorOptions options);

    bool isOperational() const override { return true; }

    api::Result detect(cv::Mat const &image) override;

    std::string getName() const override;

    api::DetectorType getType() const override;
  };
}
