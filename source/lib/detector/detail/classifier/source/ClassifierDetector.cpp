
#include "../include/ClassifierDetector.h"

#include "lib/detector/detail/common/include/DetectorPipe.h"

#include "lib/dip/include/Transform.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/objdetect.hpp> // Reduce include dependencies here

#include <filesystem>

namespace dip::detection::api
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

  ClassifierDetector::ClassifierDetector(infrastructure::Context &context, DetectorOptions options)
      : logger(CREATE_LOGGER(context.getLoggerFactory())), internal(options.classifierFile
                                                                        ? std::make_shared<Internal>(*options.classifierFile)
                                                                        : std::unique_ptr<Internal>()) {}

  bool ClassifierDetector::isOperational() const
  {
    return (bool)internal;
  }

  std::string ClassifierDetector::getName() const { return "Classifier"; }

  DetectorType ClassifierDetector::getType() const { return DetectorType::CLASSIFIER_DETECTOR; }

  Result ClassifierDetector::detect(cv::Mat const &input)
  {
    using cd = detail::DetectorPipe;

    if (!internal)
    {
      return Result({});
    }

    auto preProcessedImage = dip::filtering::toGray(input);
    auto objects = std::vector<cv::Rect>{};
    internal->classifier->detectMultiScale(preProcessedImage, objects);

    auto const minimalSize = 150. * 150.;
    auto descriptor = cd::filter(
        detail::DetectorPipeDescriptor::fromContours(Descriptor::fromRects(std::move(objects))),
        {
            cd::removeIf(cd::areaSmallerThan(minimalSize)), //
            cd::sortBy(cd::biggestArea()),                  //
            // cd::removeIfParent(),                           //
            // cd::removeIf(cd::emptyImage()),
            cd::annotateWith({cd::dimensionString()}),
            /* cd::printTo(std::cout) */
        });

    return Result{std::move(descriptor.contours)};
  }
}
