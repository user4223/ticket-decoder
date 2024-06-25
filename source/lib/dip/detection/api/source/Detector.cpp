
#include "../include/Detector.h"

#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ForwardDetector.h"

#include <map>
#include <functional>

namespace dip::detection::api
{
  DetectorOptions defaultOptions = DetectorOptions{};

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, DetectorType type)
  {
    return create(loggerFactory, type, defaultOptions);
  }

  static std::map<DetectorType, std::function<std::unique_ptr<Detector>(::utility::LoggerFactory &, DetectorOptions)>> factoryMap = {
      {DetectorType::NOP_FORWARDER, [](auto &loggerFactory, auto options)
       { return std::unique_ptr<Detector>{new ForwardDetector(loggerFactory, std::move(options))}; }},
      {DetectorType::SQUARE_DETECTOR, [](auto &loggerFactory, auto options)
       { return std::unique_ptr<Detector>{new SquareDetector(loggerFactory, std::move(options))}; }},
      {DetectorType::CLASSIFIER, [](auto &loggerFactory, auto options)
       { return std::unique_ptr<Detector>{new ClassifierDetector(loggerFactory, std::move(options))}; }}};

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, DetectorType type, DetectorOptions options)
  {
    auto entry = factoryMap.find(type);
    if (entry == factoryMap.end())
    {
      throw std::runtime_error("Detector type not implemented");
    }
    return entry->second(loggerFactory, std::move(options));
  }

  std::vector<std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory)
  {
    return createAll(loggerFactory, defaultOptions);
  }

  std::vector<std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory, DetectorOptions options)
  {
    auto detectors = std::vector<std::shared_ptr<Detector>>{};
    std::transform(factoryMap.begin(), factoryMap.end(), std::back_inserter(detectors), [&](auto &entry)
                   { return entry.second(loggerFactory, options); });
    return detectors;
  }
}
