
#include "../include/Detector.h"

#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ForwardDetector.h"

namespace dip::detection::api
{
  Parameters defaultParameters = Parameters{};

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, Type type)
  {
    return create(loggerFactory, type, defaultParameters);
  }

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, Type type, Parameters &parameters)
  {
    return std::make_unique<ForwardDetector>(loggerFactory, parameters);
  }

  std::vector<std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory)
  {
    return createAll(loggerFactory, defaultParameters);
  }

  std::vector<std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory, Parameters &parameters)
  {
    return {
        std::unique_ptr<Detector>{new SquareDetector(loggerFactory, parameters)},
        std::unique_ptr<Detector>{new ClassifierDetector(loggerFactory, parameters)},
        std::unique_ptr<Detector>{new ForwardDetector(loggerFactory, parameters)}};
  }
}
