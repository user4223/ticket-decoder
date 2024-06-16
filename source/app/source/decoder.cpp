
#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/Utility.h"

#include "lib/api/include/DecoderFacade.h"

#include <tclap/CmdLine.h>

#include <ostream>

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
      "Decode aztec code and dump raw data to output after base64 encoding",
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
      false, true, "Boolean flag", cmd);
  auto imageRotationArg = TCLAP::ValueArg<int>(
      "", "rotate-image",
      "Rotate input image before processing for the given amount of degrees (default 0)",
      false, 0, "Integer value", cmd);
  auto imageScaleArg = TCLAP::ValueArg<int>(
      "", "scale-image",
      "Scale input image before processing in percent (default 100)",
      false, 100, "Integer value", cmd);
  auto imageSplitArgContraintValues = std::vector<std::string>({"11", "21", "22", "41", "42", "43", "44"});
  auto imageSplitArgContraint = TCLAP::ValuesConstraint<std::string>(imageSplitArgContraintValues);
  auto imageSplitArg = TCLAP::ValueArg<std::string>(
      "", "split-image",
      "Split input image, 1st number specifies the no of parts to split, 2nd is the part used for processing, clockwise from top/left (default 11)",
      false, "11", &imageSplitArgContraint, cmd);
  auto imageFlipArgContraintValues = std::vector<unsigned int>({0u, 1u, 2u, 3u});
  auto imageFlipArgContraint = TCLAP::ValuesConstraint<unsigned int>(imageFlipArgContraintValues);
  auto imageFlipArg = TCLAP::ValueArg<unsigned int>(
      "", "flip-image",
      "Flip input image around X if 1, around Y if 2, and around X and Y if 3 (default 0)",
      false, 0u, &imageFlipArgContraint, cmd);

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

  if (inputPathArg.isSet() && outputPathArg.isSet())
  {
    io::api::utility::checkAndEnsureCompatiblePaths(inputPathArg.getValue(), outputPathArg.getValue());
  }

  auto loggerFactory = ::utility::LoggerFactory::create(verboseArg.getValue());
  auto decoderFacade = api::DecoderFacade::create(loggerFactory)
                           .withPureBarcode(pureBarcodeArg.getValue())
                           .withLocalBinarizer(binarizerEnabledArg.getValue())
                           .withPublicKeyFile(publicKeyFilePathArg.getValue())
                           .withImageRotation(imageRotationArg.getValue())
                           .withImageScale(imageScaleArg.getValue())
                           .withImageSplit(imageSplitArg.getValue())
                           .withImageFlipping(imageFlipArg.getValue())
                           .withDetectorType(dip::detection::api::DetectorType::NOP_FORWARDER)
                           .withFailOnInterpretationError(true)
                           .build();

  auto const inputPath = std::filesystem::path(inputPathArg.getValue());
  auto output = outputPathArg.isSet()
                    ? io::api::utility::OutputStream(outputPathArg.getValue())
                    : io::api::utility::OutputStream();

  if (rawUIC918FilePathArg.isSet())
  {
    output.get() << decoderFacade.decodeRawFileToJson(rawUIC918FilePathArg.getValue())
                 << std::endl;
    return 0;
  }

  if (base64EncodedUIC918Data.isSet())
  {
    output.get() << decoderFacade.decodeRawBase64ToJson(base64EncodedUIC918Data.getValue())
                 << std::endl;
    return 0;
  }

  if (outputBase64RawDataArg.getValue())
  {
    auto const rawElements = decoderFacade.decodeFileToRawBase64(inputPath);
    std::for_each(rawElements.begin(), rawElements.end(), [&](auto &&rawElement)
                  { output.get() << rawElement << std::endl; });
    return 0;
  }

  auto const jsonElements = decoderFacade.decodeFileToJson(inputPath);
  std::for_each(jsonElements.begin(), jsonElements.end(), [&](auto &&jsonElement)
                { output.get() << jsonElement << std::endl; });
  return 0;
}
