
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

int main(int argc, char **argv)
{
   auto imageSource = dip::utility::ImageSource::create("../../images/", 1u);
   auto const outBasePath = std::filesystem::path("../../out/");

   auto dump = true, overlayOutputImage = true, pure = false;
   auto detectorIndex = 0u;
   auto parameters = dip::detection::api::Parameters{7, 17};

   auto const detectors = std::vector<std::shared_ptr<dip::detection::api::Detector>>{
       dip::detection::api::SquareDetector::create(parameters),
       dip::detection::api::ClassifierDetector::create(),
       dip::detection::api::ResearchDetector::create(parameters)};

   auto const keyMapper = utility::KeyMapper(10, // clang-format off
   {    
       {'i', [&](){ return "i: " + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "I: " + std::to_string(utility::safeDecrement(parameters.imageProcessingDebugStep)); }},
       {'c', [&](){ return "c: " + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "C: " + std::to_string(utility::safeDecrement(parameters.contourDetectorDebugStep)); }},
       {'f', [&](){ return "f: " + imageSource.nextSource(); }},
       {'F', [&](){ return "F: " + imageSource.previousSource(); }},
       {'r', [&](){ return "r: " + imageSource.rotateCounterClockwise(); }},
       {'R', [&](){ return "R: " + imageSource.rotateClockwise(); }},
       {'d', [&](){ return "detector: " + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
       {'D', [&](){ dump = !dump; return "dump: " + std::to_string(dump); }},
       {'p', [&](){ return "pure barcode: " + std::to_string(pure = !pure); }},
       {'o', [&](){ return "overlay output image: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
       {'2', [&](){ return "2: " + imageSource.togglePart2(); }},
       {'4', [&](){ return "4: " + imageSource.togglePart4(); }},
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
                     {  return barcode::api::Decoder::decode(contourDescriptor, pure); });

      auto interpreterResults = std::vector<std::optional<std::string>>{};
      std::transform(decodingResults.begin(), decodingResults.end(),
                     std::back_inserter(interpreterResults),
                     [&](auto const &decodingResult)
                     {  return uic918::api::Interpreter::interpret(decodingResult.payload, 3); });

      if (dump && (!source.path || keyHandled)) 
      {
         auto const ouputPath = std::filesystem::path(outBasePath).append(source.annotation);
         std::for_each(decodingResults.begin(), decodingResults.end(),
                       [&](auto const &result)
                       {  barcode::api::dump(ouputPath, result); });
      }

      auto outputImage = dip::filtering::toColor(detectionResult.debugImage.value_or(source.image));
      auto const outputContours = detectionResult.debugContours.value_or(detectionResult.contours);
      std::for_each(outputContours.begin(), outputContours.end(), 
                    [&](auto const &descriptor)
                    { 
                      if (overlayOutputImage) dip::utility::copyTo(outputImage, descriptor.image, descriptor.square);
                      dip::utility::drawRedShape(outputImage, descriptor.contour);
                      dip::utility::putBlueText(outputImage, descriptor.evaluateAnnotations()); });

      std::for_each(decodingResults.begin(), decodingResults.end(),
                    [&](auto const &decodingResult)
                    {  
                      dip::utility::drawShape(outputImage, decodingResult.box, barcode::api::getProperties(decodingResult.level)); 
                      std::cout << barcode::api::getCharacter(decodingResult.level) << std::flush; });

      std::for_each(interpreterResults.begin(), interpreterResults.end(),
                     [&](auto const &interpreterResult)
                     {  dip::utility::putRedText(outputImage, cv::Point(5, 140), 35, interpreterResult.value_or("")); });
      
      auto outputLines = std::vector<std::string>{source.annotation};
      parameters.to_string(std::back_inserter(outputLines));
      dip::utility::putRedText(outputImage, cv::Point(5, 35), 35, outputLines);
      dip::utility::putBlueDimensions(outputImage);
      dip::utility::showImage(outputImage); });

   return 0;
}
