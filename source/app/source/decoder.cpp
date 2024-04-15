
#include "lib/utility/include/Logging.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Base64.h"

#include "lib/dip/utility/include/PreProcessor.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/barcode/api/include/Decoder.h"
#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Interpreter.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include <tclap/CmdLine.h>

#include <ostream>
#include <fstream>

int main(int argc, char **argv)
{
  auto cmd = TCLAP::CmdLine("ticket-decoder", ' ', "v0.1");
  auto verboseArg = TCLAP::SwitchArg(
      "v", "verbose",
      "More verbose debug logging",
      cmd, false);
  auto imageFilePathArg = TCLAP::ValueArg<std::string>(
      "i", "image-file",
      "Path to image file containing aztec code to process",
      true, "", "File path [png, jpeg]");
  auto rawUIC918FilePathArg = TCLAP::ValueArg<std::string>(
      "r", "raw-file",
      "Path to binary file containing UIC918 raw data to process",
      true, "", "File path");
  auto base64EncodedData = TCLAP::ValueArg<std::string>(
      "b", "base64-data",
      "Base64 encoded string containing UIC918 encoded raw data to process",
      true, "", "Base64 string");
  cmd.xorAdd({&imageFilePathArg, &rawUIC918FilePathArg, &base64EncodedData});
  auto outputFilePathArg = TCLAP::ValueArg<std::string>(
      "o", "output-file",
      "Path to output file to write decoded UIC918 information to",
      false, "", "File path [json]", cmd);
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
  auto const signatureChecker = uic918::api::SignatureChecker::create(loggerFactory, publicKeyFilePathArg.getValue());
  auto const interpreter = uic918::api::Interpreter::create(loggerFactory, *signatureChecker);
  auto outputHandler = [&](auto const &interpretationResult)
  {
    if (outputFilePathArg.isSet())
      std::ofstream(outputFilePathArg.getValue(), std::ios::out | std::ios::trunc) << interpretationResult << std::endl;
    else
      std::cout << interpretationResult << std::endl;
  };

  if (rawUIC918FilePathArg.isSet())
  {
    auto const rawUIC918Data = utility::readBinary(rawUIC918FilePathArg.getValue());
    outputHandler(interpreter->interpret(rawUIC918Data, 3).value_or("{}"));
    return 0;
  }
  if (base64EncodedData.isSet())
  {
    auto const rawUIC918Data = utility::base64::decode(base64EncodedData.getValue());
    outputHandler(interpreter->interpret(rawUIC918Data, 3).value_or("{}"));
    return 0;
  }

  auto readers = io::api::Reader::create(loggerFactory, io::api::ReadOptions{});
  auto loader = io::api::Loader(loggerFactory, readers);
  auto loadResult = loader.load(imageFilePathArg.getValue());
  auto preProcessor = dip::utility::PreProcessor::create(loggerFactory, imageRotationArg.getValue(), imageSplitArg.getValue());
  auto parameters = dip::detection::api::Parameters{};
  auto const detector = dip::detection::api::Detector::create(loggerFactory, parameters);
  if (loadResult.size() < 1)
  {
    throw std::invalid_argument("File could not be processed as input properly: " + imageFilePathArg.getValue());
  }
  auto source = preProcessor.get(loadResult.get(0)); // TODO When path was a directory, process all matching files not only the first
  if (!source.isValid())
  {
    throw std::invalid_argument("File could not be processed as input properly: " + imageFilePathArg.getValue());
  }
  auto detectionResult = detector->detect(source.getImage());

  std::for_each(detectionResult.contours.begin(), detectionResult.contours.end(),
                [&](auto const &contourDescriptor)
                {
                  auto const decodingResult = barcode::api::Decoder::decode(
                      loggerFactory,
                      contourDescriptor, {pureBarcodeArg.getValue(), binarizerEnabledArg.getValue()});
                  if (outputBase64RawDataArg.getValue())
                  {
                    std::cout << utility::base64::encode(decodingResult.payload) << std::endl;
                    return;
                  }

                  outputHandler(interpreter->interpret(decodingResult.payload, 3).value_or("{}"));
                });

  return 0;
}
