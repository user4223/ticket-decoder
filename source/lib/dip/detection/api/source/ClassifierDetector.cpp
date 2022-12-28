
#include "../include/ClassifierDetector.h"

#include "../../detail/include/Pipe.h"

#include "lib/dip/filtering/include/Transform.h"

#include "lib/utility/include/Logging.h"

#include <opencv2/opencv.hpp> // Reduce include dependencies here

#include <filesystem>

namespace dip::detection::api
{
  struct ClassifierDetector::Internal
  {
    std::unique_ptr<cv::CascadeClassifier> classifier;

    Internal(std::filesystem::path programFolderPath)
    {
      // TODO: Provide a proper classification file!

      // std::string const file = "etc/aztec-classifier.xml";
      auto const file = programFolderPath / "etc" / "haarcascade_frontalface_default.xml";
      if (!std::filesystem::exists(file))
      {
        throw std::domain_error("Required classifier file not found: " + file.string());
      }
      classifier = std::make_unique<cv::CascadeClassifier>(file);
    }
  };

  ClassifierDetector::ClassifierDetector(::utility::LoggerFactory &loggerFactory, Parameters &parameters)
      : logger(CREATE_LOGGER(loggerFactory)), internal(std::make_shared<Internal>(parameters.programFolderPath)) {}

  std::string ClassifierDetector::getName() { return "Classifier"; }

  Result ClassifierDetector::detect(cv::Mat const &input)
  {
    using cd = detail::Pipe;

    auto preProcessedImage = dip::filtering::toGray(input);
    auto objects = std::vector<cv::Rect>{};
    internal->classifier->detectMultiScale(preProcessedImage, objects);

    auto const minimalSize = input.rows * input.cols * (1. / 150.);
    auto descriptor = cd::filter(
        detail::PipeDescriptor::fromContours(Descriptor::fromRects(std::move(objects))),
        {
            cd::removeIf(cd::areaSmallerThan(minimalSize)), //
            cd::sortBy(cd::biggestArea()),                  //
            cd::removeIfParent(),                           //
            cd::annotateWith({cd::dimensionString()}),
            /* cd::printTo(std::cout) */
        });

    return Result{std::move(descriptor.contours)};
  }
}
