
#include "../include/Detector.h"

#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ForwardDetector.h"

#include <map>
#include <functional>

namespace dip::detection::api
{
  Parameters defaultParameters = Parameters{};

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, DetectorType type)
  {
    return create(loggerFactory, type, defaultParameters);
  }

  static std::map<DetectorType, std::function<std::unique_ptr<Detector>(::utility::LoggerFactory &, Parameters &)>> factoryMap = {
      {DetectorType::NOP_FORWARDER, [](auto &loggerFactory, auto &parameters)
       { return std::unique_ptr<Detector>{new ForwardDetector(loggerFactory, parameters)}; }},
      {DetectorType::SQUARE_DETECTOR, [](auto &loggerFactory, auto &parameters)
       { return std::unique_ptr<Detector>{new SquareDetector(loggerFactory, parameters)}; }},
      {DetectorType::CLASSIFIER, [](auto &loggerFactory, auto &parameters)
       { return std::unique_ptr<Detector>{new ClassifierDetector(loggerFactory, parameters)}; }}};

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, DetectorType type, Parameters &parameters)
  {
    auto entry = factoryMap.find(type);
    if (entry == factoryMap.end())
    {
      throw std::runtime_error("Detector type not implemented");
    }
    return entry->second(loggerFactory, parameters);
  }

  std::vector<std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory)
  {
    return createAll(loggerFactory, defaultParameters);
  }

  std::vector<std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory, Parameters &parameters)
  {
    auto detectors = std::vector<std::shared_ptr<Detector>>{};
    std::transform(factoryMap.begin(), factoryMap.end(), std::back_inserter(detectors), [&](auto &entry)
                   { return entry.second(loggerFactory, parameters); });
    return detectors;
  }
}
