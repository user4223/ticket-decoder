
#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ResearchDetector.h"
#include "lib/dip/detection/api/include/Utility.h"
#include "lib/dip/filtering/include/Transform.h"

#include "lib/barcode/api/include/Decoder.h"
#include "lib/barcode/api/include/Utility.h"

#include "lib/uic918/api/include/Interpreter.h"

#include "lib/utility/include/KeyMapper.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"
#include "lib/utility/include/Camera.h"
#include "lib/utility/include/Window.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <memory>
#include <iostream>
#include <filesystem>
#include <numeric>
#include <algorithm>
#include <map>

int main(int argc, char **argv)
{
   auto const imagePaths = utility::scanForImages("../../images/");
   auto const outBasePath = std::filesystem::path("../../out/");
   auto parts = std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 2u}};
   auto imageCache = std::map<std::string, cv::Mat>{};

   auto dump = true, overlayOutputImage = true, pure = false;
   auto inputFileIndex = 1u, rotationDegree = 0u, detectorIndex = 0u;
   auto parameters = dip::detection::api::Parameters{7, 17};

   auto const detectors = std::vector<std::shared_ptr<dip::detection::api::Detector>>{
       dip::detection::api::SquareDetector::create(parameters),
       dip::detection::api::ClassifierDetector::create(),
       dip::detection::api::ResearchDetector::create(parameters)};

   auto const keyMapper = utility::KeyMapper( // clang-format off
   {    
       {'i', [&](){ return "i: " + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "I: " + std::to_string(utility::safeDecrement(parameters.imageProcessingDebugStep)); }},
       {'c', [&](){ return "c: " + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "C: " + std::to_string(utility::safeDecrement(parameters.contourDetectorDebugStep)); }},
       {'f', [&](){ return "f: " + std::to_string(utility::safeIncrement(inputFileIndex, imagePaths.size())); }},
       {'F', [&](){ return "F: " + std::to_string(utility::safeDecrement(inputFileIndex)); }},
       {'r', [&](){ return "r: " + std::to_string(utility::safeIncrement(rotationDegree, 5, 360)); }},
       {'R', [&](){ return "R: " + std::to_string(utility::safeDecrement(rotationDegree, 5)); }},
       {'d', [&](){ return "detector: " + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
       {'D', [&](){ dump = !dump; return "dump: " + std::to_string(dump); }},
       {'p', [&](){ return "pure barcode: " + std::to_string(pure = !pure); }},
       {'o', [&](){ return "overlay output image: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
       {'2', [&](){ return "2: " + std::to_string(utility::rotate(parts.at(2), 2)); }},
       {'4', [&](){ return "4: " + std::to_string(utility::rotate(parts.at(4), 4)); }},
   }); // clang-format on

   keyMapper.handle(std::cout, [&]()
                    {
      auto const inputPath = inputFileIndex == 0 || imagePaths.empty()
                                 ? std::nullopt
                                 : std::make_optional(imagePaths[std::min((unsigned int)(imagePaths.size()), inputFileIndex) - 1]);

      auto input = cv::Mat{};
      auto inputAnnotation = std::optional<std::string>();
      auto outPath = std::filesystem::path();
      if (inputPath)
      {
         utility::Camera::release();
         inputAnnotation = inputPath->filename();
         outPath = std::filesystem::path(outBasePath).append(inputPath->stem().string());

         auto const cacheEntry = imageCache.find(inputPath->string());
         if (cacheEntry == imageCache.end()) {
            auto const entry = imageCache.insert(std::make_tuple(inputPath->string(), cv::imread(inputPath->string(), cv::IMREAD_COLOR)));
            input = entry.first->second.clone();   
         } else {
            input = cacheEntry->second.clone();
         }

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
         input = utility::Camera::getImage();
      }

      if (input.empty())
      {
         return;
      }

      auto &detector = *detectors[detectorIndex];
      auto detectorResult = detector.detect(input);

      auto decodingResults = std::vector<barcode::api::Result>{};
      std::transform(detectorResult.contours.begin(),
                     detectorResult.contours.end(),
                     std::inserter(decodingResults, decodingResults.begin()),
                     [&](auto const &descriptor)
                     { 
                        auto result = barcode::api::Decoder::decode(descriptor.id, descriptor.square, descriptor.image, pure);
                        if (dump) 
                        {
                           barcode::api::dump(outPath, result);
                        }
                        barcode::api::visualize(std::cout, result); 
                        return result; });

      auto output = detectorResult.debugImage.value_or(input);
      dip::detection::api::visualize(
         output,
         detectorResult.debugContours.value_or(detectorResult.contours), 
         overlayOutputImage);

      output = std::reduce(decodingResults.begin(),
                                decodingResults.end(),
                                std::move(output),
                                [&](auto &&image, barcode::api::Result const &result)
                                {
                                   if (result.isDecoded())
                                   {
                                      auto const json = uic918::api::Interpreter::interpretPretty(result.payload);
                                      if (json)
                                      {
                                         cv::putText(image, *json, cv::Point(0, 140), cv::FONT_HERSHEY_SIMPLEX, 1., cv::Scalar(0, 0, 255), 2);
                                      }
                                   }
                                   barcode::api::visualize(image, result); 
                                   return image; });
      if (inputAnnotation)
      {
         cv::putText(output, inputAnnotation.value(), cv::Point(0, 70), cv::FONT_HERSHEY_SIMPLEX, 1., cv::Scalar(0, 0, 255), 2);
      }

      utility::Window::show(output); });

   return 0;
}
