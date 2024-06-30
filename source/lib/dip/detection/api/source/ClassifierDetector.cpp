
#include "../include/ClassifierDetector.h"

#include "../../detail/include/Pipe.h"

#include "lib/dip/filtering/include/Transform.h"

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

  ClassifierDetector::ClassifierDetector(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DetectorOptions options)
      : logger(CREATE_LOGGER(loggerFactory)), internal(options.classifierFile
                                                           ? std::make_shared<Internal>(*options.classifierFile)
                                                           : std::unique_ptr<Internal>()) {}

  bool ClassifierDetector::isOperational() const
  {
    return (bool)internal;
  }

  std::string ClassifierDetector::getName() { return "Classifier"; }

  Result ClassifierDetector::detect(cv::Mat const &input)
  {
    using cd = detail::Pipe;

    if (!internal)
    {
      return Result({});
    }

    auto preProcessedImage = dip::filtering::toGray(input);
    auto objects = std::vector<cv::Rect>{};
    internal->classifier->detectMultiScale(preProcessedImage, objects);

    auto const minimalSize = 150. * 150.;
    auto descriptor = cd::filter(
        detail::PipeDescriptor::fromContours(Descriptor::fromRects(std::move(objects))),
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
