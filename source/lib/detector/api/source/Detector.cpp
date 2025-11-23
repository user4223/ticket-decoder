// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Detector.h"
#include "../include/NopDetector.h"

#ifdef WITH_SQUARE_DETECTOR
#include "lib/detector/detail/square/include/SquareDetector.h"
#endif
#ifdef WITH_CLASSIFIER_DETECTOR
#include "lib/detector/detail/classifier/include/ClassifierDetector.h"
#endif

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include <map>
#include <functional>

namespace detector::api
{
  using CreatorType = std::function<std::unique_ptr<Detector>(infrastructure::Context &, DetectorOptions)>;

  template <typename T>
  CreatorType creator()
  {
    return [](auto &context, auto options)
    { return std::unique_ptr<Detector>{new T(context, std::move(options))}; };
  }

  static std::map<DetectorType, CreatorType> factoryMap =
      {
          {DetectorType::NOP_DETECTOR, creator<NopDetector>()},
#ifdef WITH_SQUARE_DETECTOR
          {DetectorType::SQUARE_DETECTOR, creator<detail::SquareDetector>()},
#endif
#ifdef WITH_CLASSIFIER_DETECTOR
          {DetectorType::CLASSIFIER_DETECTOR, creator<detail::ClassifierDetector>()},
#endif
  };

  std::unique_ptr<Detector> Detector::create(infrastructure::Context &context, DetectorType type, DetectorOptions options)
  {
    auto entry = factoryMap.find(type);
    if (entry == factoryMap.end())
    {
      throw std::runtime_error("Detector type not implemented");
    }
    auto detector = entry->second(context, std::move(options));
    if (!detector->isOperational())
    {
      throw std::runtime_error("Detector invalid, probably missing parameters: " + detector->getName());
    }
    return std::move(detector);
  }

  std::map<DetectorType, std::shared_ptr<Detector>> Detector::createAll(infrastructure::Context &context, DetectorOptions options)
  {
    auto detectors = std::map<DetectorType, std::shared_ptr<Detector>>{};
    std::for_each(factoryMap.begin(), factoryMap.end(), [&](auto &entry)
                  { 
                    auto detector = entry.second(context, options);
                    if (!detector->isOperational()) 
                    {
                      LOG_WARN(CREATE_LOGGER(context.getLoggerFactory())) << "Ignoring detector due to missing parameters: " << detector->getName();
                      return;
                    }
                    
                    detectors.emplace(entry.first, std::move(detector)); });
    return detectors;
  }

  Detector::ParameterTypeList Detector::supplyParameters() const
  {
    if (!isOperational())
    {
      return {};
    }
    return {std::make_pair("detector:", getName())};
  }
}
