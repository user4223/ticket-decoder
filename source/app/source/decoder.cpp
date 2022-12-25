
#include "lib/utility/include/Logging.h"
#include "lib/dip/utility/include/ImageSource.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/barcode/api/include/Decoder.h"
#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Interpreter.h"

#include <ostream>

int main(int argc, char **argv)
{
  auto const rootPath = std::filesystem::current_path();
  auto loggerFactory = utility::LoggerFactory::create();
  auto imageSource = dip::utility::ImageSource::create(
      loggerFactory,
      rootPath / "images" / "UIC918-9" / "Muster 918-9 CityTicket.png",
      1u, 2);
  auto parameters = dip::detection::api::Parameters{};
  auto const detector = dip::detection::api::Detector::create(loggerFactory, parameters);
  auto const signatureChecker = uic918::api::SignatureChecker::create(
      loggerFactory,
      rootPath / "cert" / "UIC_PublicKeys.xml");
  auto const interpreter = uic918::api::Interpreter::create(loggerFactory, *signatureChecker);

  auto source = imageSource.getSource();
  std::cout << source.annotation << "\n";
  auto detectionResult = detector->detect(source.image);

  std::for_each(detectionResult.contours.begin(), detectionResult.contours.end(),
                [&](auto const &contourDescriptor)
                {
                  auto const decodingResult = barcode::api::Decoder::decode(
                      loggerFactory,
                      contourDescriptor, false);
                  auto const interpretationResult = interpreter->interpret(decodingResult.payload, 3);
                  std::cout << interpretationResult.value_or("not found") << std::endl;
                });

  return 0;
}
