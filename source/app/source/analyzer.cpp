
#include "lib/dip/detection/api/include/Detector.h"

#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/filtering/include/PreProcessor.h"

#include "lib/dip/utility/include/Text.h"
#include "lib/dip/utility/include/Shape.h"
#include "lib/dip/utility/include/Color.h"
#include "lib/dip/utility/include/Image.h"

#include "lib/ui/include/Window.h"
#include "lib/ui/include/KeyMapper.h"

#include "lib/barcode/api/include/Decoder.h"
#include "lib/barcode/api/include/Utility.h"

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Utility.h"

#include "lib/utility/include/Utility.h"
#include "lib/utility/include/Logging.h"
#include "lib/utility/include/FrameRate.h"

#include "lib/io/api/include/SourceManager.h"
#include "lib/io/api/include/SinkManager.h"

#include "lib/api/include/DecoderFacade.h"

#include <nlohmann/json.hpp>

#include <tclap/CmdLine.h>

#include <memory>
#include <filesystem>
#include <numeric>
#include <algorithm>

int main(int argc, char **argv)
{
   auto cmd = TCLAP::CmdLine("ticket-analyzer", ' ', "v0.7");
   auto verboseArg = TCLAP::SwitchArg(
       "v", "verbose",
       "More verbose debug logging",
       cmd, false);
   auto inputFolderPathArg = TCLAP::ValueArg<std::string>(
       "i", "input-folder",
       "Path to folder containing input files with aztec codes to be processed",
       false, "images/", "Directory path to input files [pdf, png, jpeg]", cmd);
   auto outputFolderPathArg = TCLAP::ValueArg<std::string>(
       "o", "output-folder",
       "Path to folder to take intermediate image and raw data files and json result files",
       false, "out/", "Directory path", cmd);
   auto publicKeyFilePathArg = TCLAP::ValueArg<std::string>(
       "k", "keys-file",
       "Path to file containing public keys from UIC for signature validation",
       false, "cert/UIC_PublicKeys.xml", "File path [xml]", cmd);
   auto imageRotationArg = TCLAP::ValueArg<int>(
       "", "rotate-image",
       "Rotate input image before processing for the given amount of degrees (default 0)",
       false, 0, "Integer value", cmd);
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

   auto const outputFolderPath = std::filesystem::path(outputFolderPathArg.getValue());
   auto const inputFolderPath = std::filesystem::path(inputFolderPathArg.getValue());
   auto const executablePath = std::filesystem::canonical(std::filesystem::current_path() / argv[0]).parent_path();
   auto const classifierFile = executablePath / "etc" / "dip" / "haarcascade_frontalface_default.xml"; // TODO: This is an example, provide classification file 4 aztec codes!

   auto decoderOptions = barcode::api::DecoderOptions::DEFAULT;
   auto preProcessorOptions = dip::filtering::PreProcessorOptions::DEFAULT;

   auto loggerFactory = ::utility::LoggerFactory::create(verboseArg.getValue());
   auto decoderFacade = api::DecoderFacade::create(loggerFactory)
                            .withPureBarcode(decoderOptions.pure)
                            .withLocalBinarizer(decoderOptions.binarize)
                            .withPublicKeyFile(publicKeyFilePathArg.getValue())
                            .withImageRotation(imageRotationArg.getValue())
                            .withImageScale(preProcessorOptions.scalePercent)
                            .withImageSplit(imageSplitArg.getValue())
                            .withImageFlipping(preProcessorOptions.flippingMode)
                            .withDetectorType(dip::detection::api::DetectorType::NOP_FORWARDER)
                            .withAsynchronousLoad(true)
                            .withClassifierFile(classifierFile)
                            .build();

   auto &debugController = decoderFacade.getDebugController();
   auto sourceManager = io::api::SourceManager::create(loggerFactory, decoderFacade.load(inputFolderPath));
   auto sinkManager = io::api::SinkManager::create()
                          .useSource(inputFolderPath)
                          .useDestination(outputFolderPath)
                          .build();

   // TODO Replace preProcessor, detectors, decoder, signatureChecker, interpreter by decoderFacade usage
   auto preProcessor = dip::filtering::PreProcessor::create(loggerFactory, {imageRotationArg.getValue(),
                                                                            preProcessorOptions.scalePercent,
                                                                            imageSplitArg.getValue(),
                                                                            preProcessorOptions.flippingMode});
   auto const detectors = dip::detection::api::Detector::createAll(loggerFactory, debugController, {classifierFile});
   auto const decoder = barcode::api::Decoder::create(loggerFactory);
   auto const signatureChecker = uic918::api::SignatureChecker::create(loggerFactory, publicKeyFilePathArg.getValue());
   auto const interpreter = uic918::api::Interpreter::create(loggerFactory, *signatureChecker);

   auto dumpEnabled = true, overlayOutputImage = true, overlayOutputText = false;
   auto detectorIndex = (unsigned int)dip::detection::api::DetectorType::NOP_FORWARDER;

   auto const keyMapper = utility::KeyMapper(
       loggerFactory, 10,
       {{'i', [&]()
         { return "image step: " + std::to_string(debugController.incrementAs<unsigned int>("imageProcessingStep", 0u)); }},
        {'I', [&]()
         { return "IMAGE step: " + std::to_string(debugController.decrementAs<unsigned int>("imageProcessingStep", 0u)); }},
        {'c', [&]()
         { return "contour step: " + std::to_string(debugController.incrementAs<unsigned int>("contourDetectorStep", 0u)); }},
        {'C', [&]()
         { return "CONTOUR step: " + std::to_string(debugController.decrementAs<unsigned int>("contourDetectorStep", 0u)); }},
        {'f', [&]()
         { return "file: " + sourceManager.next(); }},
        {'F', [&]()
         { return "FILE: " + sourceManager.previous(); }},
        {' ', [&]()
         { return "camera: " + sourceManager.toggleCamera(); }},
        {'r', [&]()
         { return "rotate: " + preProcessor.rotateCCW(); }},
        {'R', [&]()
         { return "ROTATE: " + preProcessor.rotateCW(); }},
        {'2', [&]()
         { return "split 2: " + preProcessor.toggleSplit2(); }},
        {'4', [&]()
         { return "split 4: " + preProcessor.toggleSplit4(); }},
        {'s', [&]()
         { return "scale: " + preProcessor.scaleUp(); }},
        {'S', [&]()
         { return "SCALE: " + preProcessor.scaleDown(); }},
        {'x', [&]()
         { return "flip: " + preProcessor.toggleFlipping(); }},
        {'0', [&]()
         { return "reset: " + preProcessor.reset(); }},
        {'d', [&]()
         { return "detector: " + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
        {'p', [&]()
         { return "pure barcode: " + std::to_string(decoderOptions.pure = !decoderOptions.pure); }},
        {'b', [&]()
         { return "binarizer: " + std::to_string(decoderOptions.binarize = !decoderOptions.binarize); }},
        {'D', [&]()
         { return "dump: " + std::to_string(dumpEnabled = !dumpEnabled); }},
        {'o', [&]()
         { return "overlay image: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
        {'t', [&]()
         { return "overlay text: " + std::to_string(overlayOutputText = !overlayOutputText); }}});

   auto frameRate = utility::FrameRate();
   keyMapper.handle([&](bool const keyHandled)
                    {
      frameRate.update();
      auto source = sourceManager.getOrWait();
      // auto writer = sinkManager.get(source);

      auto const cameraEnabled = sourceManager.isCameraEnabled();
      if (keyHandled) preProcessor.enable(!cameraEnabled);

      source = preProcessor.get(std::move(source));

      auto &detector = *detectors.at((dip::detection::api::DetectorType)detectorIndex);
      auto detectionResult = detector.detect(source.getImage());
      
      auto decodingResults = std::vector<barcode::api::Result>{};
      std::transform(detectionResult.contours.begin(), detectionResult.contours.end(),
                     std::back_inserter(decodingResults),
                     [&](auto const &contourDescriptor)
                     {
                        auto config = cameraEnabled
                           ? barcode::api::DecoderOptions{false, decoderOptions.binarize}
                           : decoderOptions;
                        return decoder->decode(std::move(config), contourDescriptor); });

      auto interpreterResults = std::vector<std::optional<std::string>>{};
      std::transform(decodingResults.begin(), decodingResults.end(),
                     std::back_inserter(interpreterResults),
                     [&](auto const &decodingResult)
                     {  return interpreter->interpret(decodingResult.payload, source.getAnnotation(), 3); });

      if (dumpEnabled && (cameraEnabled || keyHandled)) // dump only if something changed
      {
         auto const outPath = outputFolderPath / (source.getAnnotation() + "_");
         std::accumulate(decodingResults.begin(), decodingResults.end(), 0,
                           [path = outPath](auto index, auto const &decodingResult) mutable
                           {  
                              barcode::api::dump(path += std::to_string(index), decodingResult);
                              return index + 1; });
         std::accumulate(interpreterResults.begin(), interpreterResults.end(), 0,
                           [path = outPath](auto index, auto const & interpreterResult) mutable
                           { 
                              uic918::api::dump(path += std::to_string(index), interpreterResult.value_or("{}"));
                              return index + 1; });
      }

      auto outputImage = dip::filtering::toColor(detectionResult.debugImage.value_or(source.getImage()));
      auto const outputContours = detectionResult.debugContours.value_or(detectionResult.contours);
      std::for_each(outputContours.begin(), outputContours.end(), 
                    [&](auto const &descriptor)
                    { 
                      if (overlayOutputImage)
                      {
                        auto const roi = cv::Rect(
                           std::max(descriptor.image.cols - descriptor.square.width, 0) / 2, 
                           std::max(descriptor.image.rows - descriptor.square.height, 0) / 2, 
                           descriptor.square.width, descriptor.square.height);
                        auto const intersection = cv::Rect({}, descriptor.image.size()) & roi;
                        dip::utility::copyTo(outputImage, descriptor.image(intersection), descriptor.square);
                      }
                      dip::utility::drawRedShape(outputImage, descriptor.contour);
                      dip::utility::drawBlueText(outputImage, descriptor.evaluateAnnotations()); });

      std::for_each(decodingResults.begin(), decodingResults.end(),
                    [&](auto const &decodingResult)
                    {   dip::utility::drawShape(outputImage, decodingResult.box, barcode::api::getDrawProperties(decodingResult.level)); });

      if (overlayOutputText) 
      {
         std::for_each(interpreterResults.begin(), interpreterResults.end(),
                       [&](auto const &interpreterResult)
                       {   dip::utility::drawRedText(outputImage, cv::Point(5, 10 * 35), 35, interpreterResult.value_or("{}")); });
      }

      auto const anyValidated = std::any_of(interpreterResults.begin(), interpreterResults.end(), [](auto const& interpreterResult)
      { 
         auto const result = nlohmann::json::parse(interpreterResult.value_or("{}"));
         return !result.empty() && result.contains("validated") && result.at("validated") == "true";
      });
      
      auto outputLines = std::vector<std::pair<std::string, std::string>>{};
      sourceManager.toString(std::back_inserter(outputLines));
      preProcessor.toString(std::back_inserter(outputLines));
      detector.toString(std::back_inserter(outputLines));
      debugController.toString(std::back_inserter(outputLines));
      frameRate.toString(std::back_inserter(outputLines));
      auto const lineCount = dip::utility::drawRedText(outputImage, cv::Point(5, 35), 35, 200, outputLines);

      dip::utility::drawShape(outputImage, cv::Rect(outputImage.cols - 60, 50, 30, 30),
            dip::utility::Properties{anyValidated ? dip::utility::green : dip::utility::red, -1});
      dip::utility::drawBlueText(outputImage, dip::utility::getDimensionAnnotations(outputImage));
      dip::utility::showImage(outputImage); });

   return 0;
}
