#pragma once

#include "DetectorOptions.h"
#include "Detector.h"

#include "lib/utility/include/LoggingFwd.h"

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
    ForwardDetector(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorOptions options);

    bool isOperational() const override { return true; }

    Result detect(cv::Mat const &image) override;

    std::string getName() override;
  };
}
