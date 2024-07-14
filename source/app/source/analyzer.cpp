
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
#include <sstream>

class OutputCollider
{
   using OutLineType = std::vector<std::pair<std::string, std::string>>;

   io::api::SinkManager sinkManager;
   std::optional<io::api::Writer> writer;
   utility::FrameRate frameRate;

   bool inputChanged = true;
   bool validated = false;

   std::optional<std::function<cv::Mat()>> fallbackOutputImageSupplier;
   cv::Mat outputImage;
   OutLineType outputLines;

public:
   bool overlayText = true;
   bool overlayImage = true;
   bool dumpResults = true;

   OutputCollider(io::api::SinkManager sm)
       : sinkManager(std::move(sm))
   {
   }

   void reset(bool ic, std::function<void(OutLineType &)> adder)
   {
      inputChanged = ic;
      validated = false;
      fallbackOutputImageSupplier = std::nullopt;
      outputLines = {};

      frameRate.update();
      frameRate.toString(std::back_inserter(outputLines));
      adder(outputLines);
   }

   void handlePreProcessorResult(io::api::InputElement const &preProcessorResult)
   {
      fallbackOutputImageSupplier = std::make_optional([&]()
                                                       { return preProcessorResult.getImage(); });

      if (dumpResults && inputChanged)
      {
         writer = sinkManager.get(preProcessorResult);
      }
   }

   void handleDetectorResult(dip::detection::api::Result const &result)
   {
      if (overlayImage && result.debugImage)
      {
         outputImage = dip::filtering::toColor(result.debugImage->clone());
      }
      else
      {
         outputImage = dip::filtering::toColor(fallbackOutputImageSupplier.value()());
      }

      auto const outputContours = result.debugContours.value_or(result.contours);
      std::for_each(outputContours.begin(), outputContours.end(),
                    [&](auto const &descriptor)
                    {
                       if (overlayImage)
                       {
                          auto const roi = cv::Rect(
                              std::max(descriptor.image.cols - descriptor.square.width, 0) / 2,
                              std::max(descriptor.image.rows - descriptor.square.height, 0) / 2,
                              descriptor.square.width, descriptor.square.height);
                          auto const intersection = cv::Rect({}, descriptor.image.size()) & roi;
                          dip::utility::copyTo(outputImage, descriptor.image(intersection), descriptor.square);
                       }
                       dip::utility::drawRedShape(outputImage, descriptor.contour);
                       dip::utility::drawBlueText(outputImage, descriptor.evaluateAnnotations());
                    });
   }

   void handleDecoderResult(barcode::api::Result const &result)
   {
      dip::utility::drawShape(outputImage, result.box, barcode::api::getDrawProperties(result.level));

      if (dumpResults && inputChanged)
      {
         if (result.isDecoded())
         {
            writer->write(result.payload);
         }

         if (!result.image.empty())
         {
            auto postfix = std::string{};
            switch (result.level)
            {
            case barcode::api::Level::Decoded:
               postfix += "decoded";
               break;
            case barcode::api::Level::Detected:
               postfix += "detected";
               break;
            default:
               postfix += "failed";
               break;
            }
            writer->write(result.image, postfix);
         }
      }
   }

   void handleInterpreterResult(std::string const &result)
   {
      auto const json = nlohmann::json::parse(result);
      validated |= (!json.empty() && json.contains("validated") && json.at("validated") == "true");

      if (overlayText)
      {
         auto stream = std::stringstream{result};
         auto counter = 0u;
         for (std::string line; std::getline(stream, line, '\n') && counter < 40; ++counter)
         {
            outputLines.push_back(std::make_pair(line, ""));
         }
      }

      if (dumpResults && inputChanged)
      {
         writer->write(result);
      }
   }

   cv::Mat getImage()
   {
      dip::utility::drawBlueText(outputImage, dip::utility::getDimensionAnnotations(outputImage));
      dip::utility::drawRedText(outputImage, cv::Point(5, 35), 35, 200, outputLines);
      dip::utility::drawShape(outputImage, cv::Rect(outputImage.cols - 60, 50, 30, 30), dip::utility::Properties{validated ? dip::utility::green : dip::utility::red, -1});
      return std::move(outputImage);
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

   auto loggerFactory = ::utility::LoggerFactory::create(verboseArg.getValue());

   auto outputCollider = OutputCollider(io::api::SinkManager::create()
                                            .useSource(inputFolderPath)
                                            .useDestination(outputFolderPath)
                                            .build());

   auto decoderOptions = barcode::api::DecoderOptions::DEFAULT;
   auto preProcessorOptions = dip::filtering::PreProcessorOptions::DEFAULT;

   auto decoderFacade = api::DecoderFacade::create(loggerFactory)
                            .withPureBarcode(decoderOptions.pure)
                            .withLocalBinarizer(decoderOptions.binarize)
                            .withPublicKeyFile(publicKeyFilePathArg.getValue())
                            .withImageRotation(imageRotationArg.getValue())
                            .withImageScale(preProcessorOptions.scalePercent)
                            .withImageSplit(imageSplitArg.getValue())
                            .withImageFlipping(preProcessorOptions.flippingMode)
                            .withDetector(dip::detection::api::DetectorType::NOP_FORWARDER)
                            .withAsynchronousLoad(true)
                            .withClassifierFile(classifierFilePath)
                            .withPreProcessorResultVisitor(std::bind(&OutputCollider::handlePreProcessorResult, &outputCollider, std::placeholders::_1))
                            .withDetectorResultVisitor(std::bind(&OutputCollider::handleDetectorResult, &outputCollider, std::placeholders::_1))
                            .withDecoderResultVisitor(std::bind(&OutputCollider::handleDecoderResult, &outputCollider, std::placeholders::_1))
                            .withInterpreterResultVisitor(std::bind(&OutputCollider::handleInterpreterResult, &outputCollider, std::placeholders::_1))
                            .build();

   auto &preProcessor = decoderFacade.getPreProcessor();
   auto &debugController = decoderFacade.getDebugController();
   auto sourceManager = io::api::SourceManager::create(loggerFactory, decoderFacade.loadFiles(inputFolderPath));

   auto const detectorIndexMax = decoderFacade.getSupportetDetectorTypes().size() - 1;
   auto detectorIndex = dip::detection::api::toInt(decoderFacade.getDetector().getType());

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
         { return "detector: " + decoderFacade.setDetector(dip::detection::api::fromInt(utility::rotate(detectorIndex, detectorIndexMax))); }},
        {'p', [&]()
         { return "pure barcode: " + std::to_string(decoderOptions.pure = !decoderOptions.pure); }},
        {'b', [&]()
         { return "binarizer: " + std::to_string(decoderOptions.binarize = !decoderOptions.binarize); }},
        {'D', [&]()
         { return "dump results: " + std::to_string(outputCollider.dumpResults = !outputCollider.dumpResults); }},
        {'o', [&]()
         { return "overlay image: " + std::to_string(outputCollider.overlayImage = !outputCollider.overlayImage); }},
        {'t', [&]()
         { return "overlay text: " + std::to_string(outputCollider.overlayText = !outputCollider.overlayText); }}});

   keyMapper.handle([&](bool const keyHandled)
                    {
      auto source = sourceManager.getOrWait();
      auto const cameraEnabled = sourceManager.isCameraEnabled();

      if (keyHandled) preProcessor.enable(!cameraEnabled); // skip rotate, flip, scale and split when camera is used

      outputCollider.reset(cameraEnabled || keyHandled, [&](auto &outputLines)
      {
         sourceManager.toString(std::back_inserter(outputLines));
         decoderFacade.toString(std::back_inserter(outputLines));
         debugController.toString(std::back_inserter(outputLines));
      });

      auto const interpreterResults = decoderFacade.decodeImageToJson(std::move(source));
      
      /*
      auto config = cameraEnabled
         ? barcode::api::DecoderOptions{false, decoderOptions.binarize}
         : decoderOptions;
      return decoder->decode(std::move(config), contourDescriptor); });
      */

      dip::utility::showImage(outputCollider.getImage()); });

   return 0;
}
