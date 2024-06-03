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
    enum class Type
    {
      SQUARE_DETECTOR,
      CLASSIFIER,
      NOP_FORWARDER,
    };

    virtual ~Detector() = default;

    virtual Result detect(cv::Mat const &image) = 0;

    virtual std::string getName() = 0;

    static std::unique_ptr<Detector> create(::utility::LoggerFactory &loggerFactory, Type type);

    static std::unique_ptr<Detector> create(::utility::LoggerFactory &loggerFactory, Type type, Parameters &parameters);

    static std::vector<std::shared_ptr<Detector>> createAll(::utility::LoggerFactory &loggerFactory);

    static std::vector<std::shared_ptr<Detector>> createAll(::utility::LoggerFactory &loggerFactory, Parameters &parameters);
  };
}
