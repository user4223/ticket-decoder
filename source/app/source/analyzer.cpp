
#include "lib/dip/detection/api/include/Detector.h"

#include "lib/dip/filtering/include/Transform.h"

#include "lib/dip/utility/include/Text.h"
#include "lib/dip/utility/include/Shape.h"
#include "lib/dip/utility/include/Color.h"
#include "lib/dip/utility/include/Image.h"
#include "lib/dip/utility/include/Window.h"
#include "lib/dip/utility/include/ImageSource.h"

#include "lib/barcode/api/include/Decoder.h"
#include "lib/barcode/api/include/Utility.h"

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/uic918/api/include/SignatureChecker.h"
#include "lib/uic918/api/include/Utility.h"

#include "lib/utility/include/KeyMapper.h"
#include "lib/utility/include/Utility.h"
#include "lib/utility/include/Logging.h"

#include <nlohmann/json.hpp>

#include <tclap/CmdLine.h>

#include <memory>
#include <filesystem>
#include <numeric>
#include <algorithm>

int main(int argc, char **argv)
{
   auto cmd = TCLAP::CmdLine("ticket-analyzer", ' ', "v0.1");
   auto imageFolderPathArg = TCLAP::ValueArg<std::string>(
       "i", "image-folder",
       "Path to image folder", false, "images",
       "Path to folder containing image files containing aztec code to be analyzed", cmd);
   auto outputFolderPathArg = TCLAP::ValueArg<std::string>(
       "o", "output-folder",
       "Path to folder taking output", false, "out",
       "Path to a folder to take intermediate image processing files and transcoded information from aztec code", cmd);
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
   auto const outputFolderPath = cwd / outputFolderPathArg.getValue();

   auto loggerFactory = utility::LoggerFactory::create();
   auto imageSource = dip::utility::ImageSource::create(loggerFactory, cwd / imageFolderPathArg.getValue(), 1u, 2);

   auto parameters = dip::detection::api::Parameters{std::filesystem::canonical(cwd / argv[0]).parent_path(), 7, 17};
   auto const detectors = dip::detection::api::Detector::createAll(loggerFactory, parameters);
   auto const signatureChecker = uic918::api::SignatureChecker::create(loggerFactory, cwd / publicKeyFilePathArg.getValue());
   auto const interpreter = uic918::api::Interpreter::create(loggerFactory, *signatureChecker);

   auto dumpEnabled = true, overlayOutputImage = true, overlayOutputText = true, pureEnabled = false;
   auto detectorIndex = 2u;

   auto const keyMapper = utility::KeyMapper(loggerFactory, 10, // clang-format off
   {
       {'i', [&](){ return "image step: "    + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "IMAGE step: "    + std::to_string(utility::safeDecrement(parameters.imageProcessingDebugStep, 0)); }},
       {'c', [&](){ return "contour step: "  + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "CONTOUR step: "  + std::to_string(utility::safeDecrement(parameters.contourDetectorDebugStep, 0)); }},
       {'f', [&](){ return "file: "          + imageSource.nextSource(); }},
       {'F', [&](){ return "FILE: "          + imageSource.previousSource(); }},
       {'r', [&](){ return "rotate: "        + imageSource.rotateCounterClockwise(); }},
       {'R', [&](){ return "ROTATE: "        + imageSource.rotateClockwise(); }},
       {'2', [&](){ return "split 2: "       + imageSource.togglePart2(); }},
       {'4', [&](){ return "split 4: "       + imageSource.togglePart4(); }},
       {'s', [&](){ return "scale: "         + imageSource.upScale(); }},
       {'S', [&](){ return "SCALE: "         + imageSource.downScale(); }},
       {'0', [&](){ return "reset: "         + imageSource.reset(); }},
       {'d', [&](){ return "detector: "      + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
       {'p', [&](){ return "pure barcode: "  + std::to_string(pureEnabled = !pureEnabled); }},
       {'D', [&](){ return "dump: "          + std::to_string(dumpEnabled = !dumpEnabled); }},
       {'o', [&](){ return "overlay image: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
       {'t', [&](){ return "overlay text: "  + std::to_string(overlayOutputText = !overlayOutputText); }}
   }); // clang-format on

   keyMapper.handle([&](bool const keyHandled)
                    {
      auto source = imageSource.getSource();
      if (!source.isValid())
      {
         return;
      }

      auto detector = detectors[detectorIndex];
      auto detectionResult = detector->detect(source.image);
      
      auto decodingResults = std::vector<barcode::api::Result>{};
      std::transform(detectionResult.contours.begin(), detectionResult.contours.end(),
                     std::back_inserter(decodingResults),
                     [&](auto const &contourDescriptor)
                     {  return barcode::api::Decoder::decode(
                          loggerFactory,
                          contourDescriptor, pureEnabled); });

      auto interpreterResults = std::vector<std::optional<std::string>>{};
      std::transform(decodingResults.begin(), decodingResults.end(),
                     std::back_inserter(interpreterResults),
                     [&](auto const &decodingResult)
                     {  return interpreter->interpret(decodingResult.payload, 3); });

      if (dumpEnabled && (source.isCamera() || keyHandled)) // dump only if something changed
      {
         auto const outPath = outputFolderPath / (source.annotation + "_");
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

      auto outputImage = dip::filtering::toColor(detectionResult.debugImage.value_or(source.image));
      auto const outputContours = detectionResult.debugContours.value_or(detectionResult.contours);
      std::for_each(outputContours.begin(), outputContours.end(), 
                    [&](auto const &descriptor)
                    { 
                      if (overlayOutputImage) dip::utility::copyTo(outputImage, descriptor.image, descriptor.square);
                      dip::utility::drawRedShape(outputImage, descriptor.contour);
                      dip::utility::drawBlueText(outputImage, descriptor.evaluateAnnotations()); });

      std::for_each(decodingResults.begin(), decodingResults.end(),
                    [&](auto const &decodingResult)
                    { dip::utility::drawShape(outputImage, decodingResult.box, barcode::api::getDrawProperties(decodingResult.level)); });

      if (overlayOutputText) 
      {
         std::for_each(interpreterResults.begin(), interpreterResults.end(),
                       [&](auto const &interpreterResult)
                       { dip::utility::drawRedText(outputImage, cv::Point(5, 280), 35, interpreterResult.value_or("{}")); });
      }

      auto const anyValidated = std::any_of(interpreterResults.begin(), interpreterResults.end(), [](auto const& interpreterResult)
      { 
         auto const result = nlohmann::json::parse(interpreterResult.value_or("{}"));
         return !result.empty() && result.contains("validated") && result.at("validated") == "true";
      });
      
      auto outputLines = std::vector<std::pair<std::string, std::string>>{};
      imageSource.toString(std::back_inserter(outputLines));
      outputLines.push_back(std::make_pair("detector", detector->getName()));
      parameters.toString(std::back_inserter(outputLines));
      dip::utility::drawShape(outputImage, 
         cv::Rect(outputImage.cols - 60, 50, 30, 30), 
         dip::utility::Properties{anyValidated ? dip::utility::green : dip::utility::red, -1});
      dip::utility::drawRedText(outputImage, cv::Point(5, 35), 35, 200, outputLines);
      dip::utility::drawBlueText(outputImage, dip::utility::getDimensionAnnotations(outputImage));
      dip::utility::showImage(outputImage); });

   return 0;
}