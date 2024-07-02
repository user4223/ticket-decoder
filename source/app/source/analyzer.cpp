
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
#include <functional>

struct DebugCollector
{
   cv::Mat outputImage;

   void handle(io::api::InputElement const &source)
   {
      outputImage = dip::filtering::toColor(source.getImage());
   }
};

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
   auto const executableFolderPath = std::filesystem::canonical(std::filesystem::current_path() / argv[0]).parent_path();
   auto const classifierFilePath = executableFolderPath / "etc" / "dip" / "haarcascade_frontalface_default.xml"; // TODO: This is an example, provide classification file 4 aztec codes!

   auto decoderOptions = barcode::api::DecoderOptions::DEFAULT;
   auto preProcessorOptions = dip::filtering::PreProcessorOptions::DEFAULT;

   auto loggerFactory = ::utility::LoggerFactory::create(verboseArg.getValue());
   auto debugCollector = DebugCollector();
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
                            .withClassifierFile(classifierFilePath)
                            .withInputElementVisitor(std::bind(&DebugCollector::handle, &debugCollector, std::placeholders::_1))
                            .build();

   auto &preProcessor = decoderFacade.getPreProcessor();
   auto &debugController = decoderFacade.getDebugController();

   auto sourceManager = io::api::SourceManager::create(loggerFactory, decoderFacade.loadFiles(inputFolderPath));
   auto sinkManager = io::api::SinkManager::create()
                          .useSource(inputFolderPath)
                          .useDestination(outputFolderPath)
                          .build();

   auto dumpEnabled = true, overlayOutputImage = true, overlayOutputText = true;

   auto const detectorTypes = decoderFacade.getSupportetDetectorTypes();
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
         { return "detector: " + decoderFacade.setDetectorType((dip::detection::api::DetectorType)utility::rotate(detectorIndex, detectorTypes.size() - 1)); }},
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

      auto const interpreterResults = decoderFacade.decodeImageToJson(std::move(source));
      /*
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
                     */

      /*
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
      */

      /*
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
      */

      auto outputLines = std::vector<std::pair<std::string, std::string>>{};
      sourceManager.toString(std::back_inserter(outputLines));
      preProcessor.toString(std::back_inserter(outputLines));
      outputLines.push_back(std::make_pair("detector:", decoderFacade.getDetectorType()));
      debugController.toString(std::back_inserter(outputLines));
      frameRate.toString(std::back_inserter(outputLines));
      auto const lineCount = dip::utility::drawRedText(debugCollector.outputImage, cv::Point(5, 35), 35, 200, outputLines);

      if (overlayOutputText) 
      {
         std::for_each(interpreterResults.begin(), interpreterResults.end(),
                       [&](auto const &interpreterResult)
                       {   dip::utility::drawRedText(debugCollector.outputImage, cv::Point(5, lineCount * 35), 35, interpreterResult); });
      }

      auto const anyValidated = std::any_of(interpreterResults.begin(), interpreterResults.end(), [](auto const& interpreterResult)
      { 
         auto const result = nlohmann::json::parse(interpreterResult);
         return !result.empty() && result.contains("validated") && result.at("validated") == "true";
      });

      dip::utility::drawShape(debugCollector.outputImage, cv::Rect(debugCollector.outputImage.cols - 60, 50, 30, 30),
            dip::utility::Properties{anyValidated ? dip::utility::green : dip::utility::red, -1});
      dip::utility::drawBlueText(debugCollector.outputImage, dip::utility::getDimensionAnnotations(debugCollector.outputImage));
      dip::utility::showImage(debugCollector.outputImage); });

   return 0;
}
