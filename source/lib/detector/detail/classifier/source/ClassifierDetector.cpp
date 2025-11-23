// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/ClassifierDetector.h"

#include "lib/detector/detail/common/include/DetectorPipe.h"

#include "lib/dip/include/Transform.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/objdetect.hpp> // Reduce include dependencies here

#include <filesystem>

namespace detector::detail
{
  struct ClassifierDetector::Internal
  {
    std::unique_ptr<cv::CascadeClassifier> classifier;

    Internal(std::filesystem::path classifierFile)
    {
      if (!std::filesystem::is_regular_file(classifierFile))
      {
        throw std::domain_error("Path to classifier file does not reference a file: " + classifierFile.string());
      }
      classifier = std::make_unique<cv::CascadeClassifier>(classifierFile);
    }
  };

  ClassifierDetector::ClassifierDetector(infrastructure::Context &context, api::DetectorOptions options)
      : logger(CREATE_LOGGER(context.getLoggerFactory())), internal(options.classifierFile
                                                                        ? std::make_shared<Internal>(*options.classifierFile)
                                                                        : std::unique_ptr<Internal>()) {}

  bool ClassifierDetector::isOperational() const
  {
    return (bool)internal;
  }

  std::string ClassifierDetector::getName() const { return "Classifier"; }

  api::DetectorType ClassifierDetector::getType() const { return api::DetectorType::CLASSIFIER_DETECTOR; }

  api::Result ClassifierDetector::detect(cv::Mat const &input)
  {
    using cd = detail::DetectorPipe;

    if (!internal)
    {
      return api::Result({});
    }

    auto preProcessedImage = dip::toGray(input);
    auto objects = std::vector<cv::Rect>{};
    internal->classifier->detectMultiScale(preProcessedImage, objects);

    auto const minimalSize = 150. * 150.;
    auto descriptor = cd::filter(
        detail::DetectorPipeDescriptor::fromContours(api::Descriptor::fromRects(std::move(objects))),
        {
            cd::removeIf(cd::areaSmallerThan(minimalSize)), //
            cd::sortBy(cd::biggestArea()),                  //
            // cd::removeIfParent(),                           //
            // cd::removeIf(cd::emptyImage()),
            cd::annotateWith({cd::dimensionString()}),
            /* cd::printTo(std::cout) */
        });

    return api::Result{std::move(descriptor.contours)};
  }
}
