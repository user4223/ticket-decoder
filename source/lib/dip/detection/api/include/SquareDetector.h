#pragma once

#include "Parameters.h"
#include "Detector.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

namespace dip::detection::api
{
  class SquareDetector : public Detector
  {
    ::utility::Logger logger;
    Parameters &parameters;

  public:
    SquareDetector(::utility::LoggerFactory &loggerFactory, Parameters &parameters);

    Result detect(cv::Mat const &image) override;

    std::string getName() override;
  };
}
