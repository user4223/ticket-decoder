
#include "lib/utility/include/Logging.h"
#include "lib/dip/utility/include/ImageSource.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/barcode/api/include/Decoder.h"
#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Interpreter.h"

#include <tclap/CmdLine.h>

#include <ostream>

int main(int argc, char **argv)
{
  auto cmd = TCLAP::CmdLine("ticket-decoder", ' ', "v0.1");
  auto inputFilePathArg = TCLAP::ValueArg<std::string>(
      "i", "input-file",
      "Input image file path [png, jpeg]", true, "",
      "Path to image file containing aztec code to detect and to transcode information into json", cmd);
  auto outputFilePathArg = TCLAP::ValueArg<std::string>(
      "o", "output-file",
      "Output file path [json]", false, "",
      "Path to output file transcoded information from aztec code is written to", cmd);
  auto publicKeyFilePathArg = TCLAP::ValueArg<std::string>(
      "k", "public-key-file",
      "Public key file path [xml]", false, "cert/UIC_PublicKeys.xml",
      "Path to file containing public keys from UIC for signature validation", cmd);
  try
  {
    cmd.parse(argc, argv);
  }
  catch (TCLAP::ArgException const &e)
  {
    std::cerr << e.what() << std::endl
              << cmd.getOutput();
    return -1;
  }

  auto const cwd = std::filesystem::current_path();
  auto loggerFactory = utility::LoggerFactory::create();
  auto imageSource = dip::utility::ImageSource::create(loggerFactory, cwd / inputFilePathArg.getValue(), 1u, 2);
  if (!imageSource.isSpecificFile())
  {
    throw std::invalid_argument("Input file invalid: " + inputFilePathArg.getValue());
  }
  auto parameters = dip::detection::api::Parameters{};
  auto const detector = dip::detection::api::Detector::create(loggerFactory, parameters);
  auto const signatureChecker = uic918::api::SignatureChecker::create(loggerFactory, cwd / publicKeyFilePathArg.getValue());
  auto const interpreter = uic918::api::Interpreter::create(loggerFactory, *signatureChecker);

  auto source = imageSource.getSource();
  auto detectionResult = detector->detect(source.image);

  std::for_each(detectionResult.contours.begin(), detectionResult.contours.end(),
                [&](auto const &contourDescriptor)
                {
                  auto const decodingResult = barcode::api::Decoder::decode(
                      loggerFactory,
                      contourDescriptor, false);
                  auto const interpretationResult = interpreter->interpret(decodingResult.payload, 3).value_or("{}");
                  if (outputFilePathArg.isSet())
                  {
                    auto output = std::ofstream(cwd / outputFilePathArg.getValue(), std::ios::out | std::ios::trunc);
                    output << interpretationResult;
                  }
                  else
                  {
                    std::cout << interpretationResult << std::endl;
                  }
                });

  return 0;
}
