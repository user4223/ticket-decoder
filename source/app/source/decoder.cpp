
#include "lib/utility/include/Logging.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Base64.h"

#include "lib/dip/filtering/include/PreProcessor.h"

#include "lib/dip/detection/api/include/Detector.h"

#include "lib/barcode/api/include/Decoder.h"

#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Interpreter.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/SinkManager.h"
#include "lib/io/api/include/Utility.h"

#include <tclap/CmdLine.h>

#include <ostream>
#include <fstream>

int main(int argc, char **argv)
{
  auto cmd = TCLAP::CmdLine("ticket-decoder", ' ', "v0.7");
  auto verboseArg = TCLAP::SwitchArg(
      "v", "verbose",
      "More verbose debug logging",
      cmd, false);
  auto inputPathArg = TCLAP::ValueArg<std::string>(
      "i", "input-path",
      "Path to input file or directory containing aztec codes to process",
      true, "", "File or directory path [pdf, png, jpeg]");
  auto rawUIC918FilePathArg = TCLAP::ValueArg<std::string>(
      "r", "raw-file",
      "Path to binary file containing UIC918 raw data to process",
      true, "", "File path");
  auto base64EncodedUIC918Data = TCLAP::ValueArg<std::string>(
      "b", "base64-data",
      "Base64 encoded string containing UIC918 encoded raw data to process",
      true, "", "Base64 string");
  cmd.xorAdd({&inputPathArg, &rawUIC918FilePathArg, &base64EncodedUIC918Data});
  auto outputPathArg = TCLAP::ValueArg<std::string>(
      "o", "output-path",
      "Path to output file or directory to write decoded UIC918 information to",
      false, "", "File or directory path [json]", cmd);
  auto outputBase64RawDataArg = TCLAP::SwitchArg(
      "R", "output-base64-raw-data",
      "Decode aztec code and dump raw data to stdout after base64 encoding",
      cmd, false);
  auto publicKeyFilePathArg = TCLAP::ValueArg<std::string>(
      "k", "keys-file",
      "Path to file containing public keys from UIC for signature validation",
      false, "cert/UIC_PublicKeys.xml", "File path [xml]", cmd);
  auto pureBarcodeArg = TCLAP::ValueArg<bool>(
      "P", "pure-barcode",
      "Input contains the barcode only",
      false, false, "Boolean flag", cmd);
  auto binarizerEnabledArg = TCLAP::ValueArg<bool>(
      "B", "binarizer-enabled",
      "Detector uses local average binarizer",
      false, false, "Boolean flag", cmd);
  auto imageRotationArg = TCLAP::ValueArg<int>(
      "", "rotate-image",
      "Rotate input image before processing for the given amount of degrees (default 4)",
      false, 4, "Integer value", cmd);
  auto imageSplitArgContraintValues = std::vector<std::string>({"11", "21", "22", "41", "42", "43", "44"});
  auto imageSplitArgContraint = TCLAP::ValuesConstraint<std::string>(imageSplitArgContraintValues);
  auto imageSplitArg = TCLAP::ValueArg<std::string>(
      "", "split-image",
      "Split input image, 1st number specifies the no of parts to split, 2nd is the part used for processing, clockwise from top/left (default 42)",
      false, "42", &imageSplitArgContraint, cmd);
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

  auto loggerFactory = utility::LoggerFactory::create(verboseArg.getValue());
  auto const inputPath = std::filesystem::path(inputPathArg.getValue());
  auto const optionalOutputPath = outputPathArg.isSet()
                                      ? std::make_optional(std::filesystem::path(outputPathArg.getValue()))
                                      : std::nullopt;
  // io::api::utility::checkAndEnsureInputOutputPaths(inputPath, optionalOutputPath);
  auto const decoder = barcode::api::Decoder::create(loggerFactory, barcode::api::Config{pureBarcodeArg.getValue(), binarizerEnabledArg.getValue()});
  auto const signatureChecker = uic918::api::SignatureChecker::create(loggerFactory, publicKeyFilePathArg.getValue());
  auto const interpreter = uic918::api::Interpreter::create(loggerFactory, *signatureChecker);
  auto outputHandler = [&](auto const &interpretationResult)
  {
    if (optionalOutputPath)
      std::ofstream(*optionalOutputPath, std::ios::out | std::ios::trunc) << interpretationResult << std::endl;
    else
      std::cout << interpretationResult << std::endl;
  };

  if (rawUIC918FilePathArg.isSet())
  {
    auto const rawUIC918Data = utility::readBinary(rawUIC918FilePathArg.getValue());
    outputHandler(interpreter->interpret(rawUIC918Data, rawUIC918FilePathArg.getValue(), 3)
                      .value_or("{}"));
    return 0;
  }
  if (base64EncodedUIC918Data.isSet())
  {
    auto const rawUIC918Data = utility::base64::decode(base64EncodedUIC918Data.getValue());
    outputHandler(interpreter->interpret(rawUIC918Data, base64EncodedUIC918Data.getValue(), 3)
                      .value_or("{}"));
    return 0;
  }

  auto readers = io::api::Reader::create(loggerFactory);
  auto preProcessor = dip::filtering::PreProcessor::create(loggerFactory, imageRotationArg.getValue(), imageSplitArg.getValue());
  auto sinkManager = io::api::SinkManager::create().useDestination("out/").build();
  auto const detector = dip::detection::api::Detector::create(loggerFactory, dip::detection::api::DetectorType::NOP_FORWARDER);

  io::api::Loader(loggerFactory, readers)
      .load(inputPath, [&](auto &&inputElement)
            {
                auto source = preProcessor.get(std::move(inputElement));
                if (!source.isValid())
                {
                  throw std::invalid_argument("File could not be processed as input properly: " + inputPath.string());
                }
                auto writer = sinkManager.get(source);
                auto detectionResult = detector->detect(source.getImage());

                std::for_each(detectionResult.contours.begin(), detectionResult.contours.end(),
                              [&](auto const &contourDescriptor)
                              {
                                auto const decodingResult = decoder->decode(contourDescriptor);
                                if (outputBase64RawDataArg.getValue())
                                {
                                  std::cout << utility::base64::encode(decodingResult.payload) << std::endl;
                                  return;
                                }

                                outputHandler(interpreter->interpret(decodingResult.payload, source.getAnnotation(), 3)
                                                .value_or("{}"));
                              }); });

  return 0;
}
