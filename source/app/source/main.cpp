
#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ResearchDetector.h"
#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/utility/include/Text.h"
#include "lib/dip/utility/include/Shape.h"
#include "lib/dip/utility/include/Image.h"
#include "lib/dip/utility/include/Window.h"
#include "lib/dip/utility/include/ImageSource.h"

#include "lib/barcode/api/include/Decoder.h"
#include "lib/barcode/api/include/Utility.h"

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/uic918/api/include/Utility.h"

#include "lib/utility/include/KeyMapper.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"

#include <memory>
#include <iostream>
#include <filesystem>
#include <numeric>
#include <algorithm>
#include <map>
#include <sstream>
#include <iterator>
#include <ranges>

int main(int argc, char **argv)
{
   auto imageSource = dip::utility::ImageSource::create("../../images/", 1u);
   auto const outBasePath = std::filesystem::path("../../out/");

   auto dumpEnabled = true, overlayOutputImage = true, pureEnabled = false;
   auto detectorIndex = 0u;
   auto parameters = dip::detection::api::Parameters{7, 17};

   auto const detectors = std::vector<std::shared_ptr<dip::detection::api::Detector>>{
       dip::detection::api::SquareDetector::create(parameters),
       dip::detection::api::ClassifierDetector::create(),
       dip::detection::api::ResearchDetector::create(parameters)};

   auto const keyMapper = utility::KeyMapper(10, // clang-format off
   {    
       {'i', [&](){ return "i: "              + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "I: "              + std::to_string(utility::safeDecrement(parameters.imageProcessingDebugStep, 0)); }},
       {'c', [&](){ return "c: "              + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "C: "              + std::to_string(utility::safeDecrement(parameters.contourDetectorDebugStep, 0)); }},
       {'f', [&](){ return "file: "           + imageSource.nextSource(); }},
       {'F', [&](){ return "file: "           + imageSource.previousSource(); }},
       {'r', [&](){ return "rotate: "         + imageSource.rotateCounterClockwise(); }},
       {'R', [&](){ return "rotate: "         + imageSource.rotateClockwise(); }},
       {'2', [&](){ return "split 2: "        + imageSource.togglePart2(); }},
       {'4', [&](){ return "split 4: "        + imageSource.togglePart4(); }},
       {'s', [&](){ return "scale: "          + imageSource.upScale(); }},
       {'S', [&](){ return "scale: "          + imageSource.downScale(); }},
       {'d', [&](){ return "detector: "       + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
       {'p', [&](){ return "pure barcode: "   + std::to_string(pureEnabled = !pureEnabled); }},
       {'D', [&](){ return "dump output: "    + std::to_string(dumpEnabled = !dumpEnabled); }},
       {'o', [&](){ return "overlay output: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
   }); // clang-format on

   keyMapper.handle(std::cout, [&](bool const keyHandled)
                    {
      auto source = imageSource.getSource();
      if (!source.isValid())
      {
         return;
      }

      auto &detector = *detectors[detectorIndex];
      auto detectionResult = detector.detect(source.image);

      auto decodingResults = std::vector<barcode::api::Result>{};
      std::transform(detectionResult.contours.begin(), detectionResult.contours.end(),
                     std::back_inserter(decodingResults),
                     [&](auto const &contourDescriptor)
                     {  return barcode::api::Decoder::decode(contourDescriptor, pureEnabled); });

      auto interpreterResults = std::vector<std::optional<std::string>>{};
      std::transform(decodingResults.begin(), decodingResults.end(),
                     std::back_inserter(interpreterResults),
                     [&](auto const &decodingResult)
                     {  return uic918::api::Interpreter::interpret(decodingResult.payload, 3); });

      if (dumpEnabled && (source.isCamera() || keyHandled)) // dump only if something changed
      {
         auto const outputPath = std::filesystem::path(outBasePath).append(source.annotation);
         std::accumulate(decodingResults.begin(), decodingResults.end(), 0,
                         [path = outputPath](auto index, auto const &decodingResult) mutable
                         {  
                           barcode::api::dump(path += "_" + std::to_string(index), decodingResult); 
                           return index; });
         std::accumulate(interpreterResults.begin(), interpreterResults.end(), 0,
                         [path = outputPath](auto index, auto const & interpreterResult) mutable
                         { 
                           uic918::api::dump(path += "_" + std::to_string(index), interpreterResult.value_or("{}"));
                           return index; });
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
                    {  
                      dip::utility::drawShape(outputImage, decodingResult.box, barcode::api::getDrawProperties(decodingResult.level)); 
                      std::cout << barcode::api::getCharacter(decodingResult.level) << std::flush; });

      std::for_each(interpreterResults.begin(), interpreterResults.end(),
                     [&](auto const &interpreterResult)
                     {  dip::utility::drawRedText(outputImage, cv::Point(5, 245), 35, interpreterResult.value_or("")); });
      
      auto outputLines = std::vector<std::pair<std::string, std::string>>{};
      imageSource.to_string(std::back_inserter(outputLines));
      parameters.to_string(std::back_inserter(outputLines));      
      dip::utility::drawRedText(outputImage, cv::Point(5, 35), 35, 200, outputLines);
      dip::utility::drawBlueText(outputImage, dip::utility::getDimensionAnnotations(outputImage));
      dip::utility::showImage(outputImage); });

   return 0;
}
