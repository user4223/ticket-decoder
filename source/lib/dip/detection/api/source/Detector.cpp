
#include "../include/Detector.h"

#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ForwardDetector.h"

#include <map>
#include <functional>

namespace dip::detection::api
{
  using CreatorType = std::function<std::unique_ptr<Detector>(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorOptions options)>;

  template <typename T>
  CreatorType creator()
  {
    return [](auto &loggerFactory, auto &debugController, auto options)
    { return std::unique_ptr<Detector>{new T(loggerFactory, debugController, std::move(options))}; };
  }

  static std::map<DetectorType, CreatorType> factoryMap =
      {
          {DetectorType::NOP_FORWARDER, creator<ForwardDetector>()},
          {DetectorType::SQUARE_DETECTOR, creator<SquareDetector>()},
          {DetectorType::CLASSIFIER, creator<ClassifierDetector>()}};

  static DetectorOptions defaultOptions = DetectorOptions{};

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorType type)
  {
    return create(loggerFactory, debugController, type, defaultOptions);
  }

  std::unique_ptr<Detector> Detector::create(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorType type, DetectorOptions options)
  {
    auto entry = factoryMap.find(type);
    if (entry == factoryMap.end())
    {
      throw std::runtime_error("Detector type not implemented");
    }
    auto detector = entry->second(loggerFactory, debugController, std::move(options));
    if (!detector->isOperational())
    {
      throw std::runtime_error("Detector invalid, probably missing parameters");
    }
    return std::move(detector);
  }

  std::map<DetectorType, std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController)
  {
    return createAll(loggerFactory, debugController, defaultOptions);
  }

  std::map<DetectorType, std::shared_ptr<Detector>> Detector::createAll(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorOptions options)
  {
    auto detectors = std::map<DetectorType, std::shared_ptr<Detector>>{};
    std::for_each(factoryMap.begin(), factoryMap.end(), [&](auto &entry)
                  { 
                    auto detector = entry.second(loggerFactory, debugController, options);
                    if (detector->isOperational()) 
                    {
                      detectors.emplace(entry.first, std::move(detector));
                    } });
    return detectors;
  }
}
