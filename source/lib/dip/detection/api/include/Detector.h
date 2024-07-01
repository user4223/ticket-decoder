#pragma once

#include "DetectorType.h"
#include "Result.h"
#include "DetectorOptions.h"

#include "lib/utility/include/DebugController.h"
#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <string>
#include <memory>
#include <map>

namespace dip::detection::api
{
  class Detector
  {
  public:
    virtual ~Detector() = default;

    virtual Result detect(cv::Mat const &image) = 0;

    virtual std::string getName() = 0;

    virtual bool isOperational() const = 0;

    static std::unique_ptr<Detector> create(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorType type);

    static std::unique_ptr<Detector> create(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorType type, DetectorOptions options);

    static std::map<DetectorType, std::shared_ptr<Detector>> createAll(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController);

    static std::map<DetectorType, std::shared_ptr<Detector>> createAll(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorOptions options);

    template <typename IteratorT>
    void toString(IteratorT inserter)
    {
      if (!isOperational())
      {
        return;
      }
      *(inserter++) = std::make_pair("detector:", getName());
    }
  };
}
