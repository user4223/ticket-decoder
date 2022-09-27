
#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ResearchDetector.h"
#include "lib/dip/detection/api/include/Utility.h"
#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/utility/include/Text.h"
#include "lib/dip/utility/include/Window.h"
#include "lib/dip/utility/include/Camera.h"
#include "lib/dip/utility/include/ImageCache.h"
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

int main(int argc, char **argv)
{
   auto imageSource = dip::utility::ImageSource::create("../../images/", 1u);
   auto const outBasePath = std::filesystem::path("../../out/");
   auto parts = std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 2u}};

   auto dump = true, overlayOutputImage = true, pure = false;
   auto rotationDegree = 0u, detectorIndex = 0u;
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
       {'r', [&](){ return "r: " + std::to_string(utility::safeIncrement(rotationDegree, 5, 360)); }},
       {'R', [&](){ return "R: " + std::to_string(utility::safeDecrement(rotationDegree, 5)); }},
       {'d', [&](){ return "detector: " + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
       {'D', [&](){ dump = !dump; return "dump: " + std::to_string(dump); }},
       {'p', [&](){ return "pure barcode: " + std::to_string(pure = !pure); }},
       {'o', [&](){ return "overlay output image: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
       {'2', [&](){ return "2: " + std::to_string(utility::rotate(parts.at(2), 2)); }},
       {'4', [&](){ return "4: " + std::to_string(utility::rotate(parts.at(4), 4)); }},
   }); // clang-format on

   keyMapper.handle(std::cout, [&](bool const keyHandled)
                    {
      auto const inputPath = imageSource.getSource();
      auto input = cv::Mat{};
      auto inputAnnotation = std::optional<std::string>();
      auto outPath = std::filesystem::path();
      if (inputPath)
      {
         dip::utility::releaseCamera();
         inputAnnotation = inputPath->filename();
         outPath = std::filesystem::path(outBasePath).append(inputPath->stem().string());
         input = dip::utility::getImage(*inputPath);

         auto const [partCount, part] = *std::max_element(
             parts.begin(),
             parts.end(),
             [](auto const &a, auto const &b)
             { return (std::min(1u, a.second) * a.first) < (std::min(1u, b.second) * b.first); });

         if (part > 0)
         {
            input = dip::filtering::split(input, partCount, part);
         }
         if (rotationDegree > 0)
         {
            input = dip::filtering::rotate(input, (float)rotationDegree);
         }
      }
      else
      {
         inputAnnotation.reset();
         outPath = std::filesystem::path(outBasePath).append("camera");
         input = dip::utility::readCamera();
      }

      if (input.empty())
      {
         return;
      }

      auto &detector = *detectors[detectorIndex];
      auto detectionResult = detector.detect(input);
      auto output = detectionResult.debugImage.value_or(input);

      auto decodingResults = std::vector<barcode::api::Result>{};
      std::transform(detectionResult.contours.begin(), detectionResult.contours.end(),
                     std::inserter(decodingResults, decodingResults.begin()),
                     [&](auto const &contourDescriptor)
                     {  return barcode::api::Decoder::decode(contourDescriptor, pure); });

      if (dump && (!inputPath || keyHandled)) 
      {
         barcode::api::dump(outPath, decodingResults);
      }

      dip::detection::api::visualize(output,
         detectionResult.debugContours.value_or(detectionResult.contours), 
         overlayOutputImage);

      barcode::api::visualize(std::cout, decodingResults);
      barcode::api::visualize(output, decodingResults);

      auto interpreterResults = std::vector<std::optional<std::string>>{};
      std::transform(decodingResults.begin(), decodingResults.end(),
                     std::inserter(interpreterResults, interpreterResults.begin()),
                     [&](auto const &decodingResult)
                     {  return uic918::api::Interpreter::interpretPretty(decodingResult.payload); });

      std::for_each(interpreterResults.begin(), interpreterResults.end(),
                     [&](auto const &interpreterResult)
                     {  dip::utility::putRedText(output, interpreterResult.value_or(""), cv::Point(0, 140)); });

      dip::utility::putRedText(output, inputAnnotation.value_or(""), cv::Point(0, 70));
      dip::utility::putBlueDimensions(output);
      dip::utility::showImage(output); });

   return 0;
}
