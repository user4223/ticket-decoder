#pragma once

#include "Result.h"
#include "Parameters.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

namespace dip::detection::api
{
  class Detector
  {
  public:
    virtual ~Detector() = default;

    virtual Result detect(cv::Mat const &image) = 0;

    virtual std::string getName() = 0;

    static std::vector<std::shared_ptr<Detector>> get(::utility::LoggerFactory &loggerFactory, Parameters &parameters);
  };
}
