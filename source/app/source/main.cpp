
#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/dip/detection/api/include/ResearchDetector.h"
#include "lib/dip/filtering/include/Transform.h"

#include "lib/barcode/api/include/Decoder.h"
#include "lib/barcode/api/include/Utility.h"

#include "lib/uic918/api/include/Interpreter.h"

#include "lib/utility/include/KeyMapper.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"
#include "lib/utility/include/Camera.h"
#include "lib/utility/include/Window.h"
#include "lib/utility/include/Visualizer.h"

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
   auto const paths = utility::scanForImages("../../images/");
   auto parts = std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 2u}};

   auto dump = true, overlayOutputImage = true, pure = false;
   auto inputFileIndex = 1u, rotationDegree = 0u, detectorIndex = 0u;
   auto inputAnnotation = std::optional<std::string>();
   auto parameters = dip::detection::api::Parameters{7, 17};

   auto const detectors = std::vector<std::shared_ptr<dip::detection::api::Detector>>{
       std::shared_ptr<dip::detection::api::Detector>(dip::detection::api::SquareDetector::create(parameters)),
       std::shared_ptr<dip::detection::api::Detector>(dip::detection::api::ClassifierDetector::create()),
       std::shared_ptr<dip::detection::api::Detector>(dip::detection::api::ResearchDetector::create(parameters))};

   auto const keyMapper = utility::KeyMapper( // clang-format off
   {    
       {'i', [&](){ return "i: " + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "I: " + std::to_string(utility::safeDecrement(parameters.imageProcessingDebugStep)); }},
       {'c', [&](){ return "c: " + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "C: " + std::to_string(utility::safeDecrement(parameters.contourDetectorDebugStep)); }},
       {'f', [&](){ return "f: " + std::to_string(utility::safeIncrement(inputFileIndex, paths.size())); }},
       {'F', [&](){ return "F: " + std::to_string(utility::safeDecrement(inputFileIndex)); }},
       {'r', [&](){ return "r: " + std::to_string(utility::safeIncrement(rotationDegree, 5, 360)); }},
       {'R', [&](){ return "R: " + std::to_string(utility::safeDecrement(rotationDegree, 5)); }},
       {'d', [&](){ return "detector: " + std::to_string(utility::rotate(detectorIndex, detectors.size() - 1)); }},
       {'p', [&](){ return "pure barcode: " + std::to_string(pure = !pure); }},
       {'o', [&](){ return "overlay output image: " + std::to_string(overlayOutputImage = !overlayOutputImage); }},
       {'2', [&](){ return "2: " + std::to_string(utility::rotate(parts.at(2), 2)); }},
       {'4', [&](){ return "4: " + std::to_string(utility::rotate(parts.at(4), 4)); }},
       {' ', [&](){ dump = !dump; return "dump: " + std::to_string(dump); }},
   }); // clang-format on

   keyMapper.handle(std::cout, [&]() // clang-format off
   {
      auto const inputPath = inputFileIndex == 0 || paths.empty()
                                 ? std::nullopt
                                 : std::make_optional(paths[std::min((unsigned int)(paths.size()), inputFileIndex) - 1]);

      cv::Mat input;
      if (inputPath)
      {
         utility::Camera::release();
         inputAnnotation = inputPath->filename();
         input = cv::imread(inputPath->string(), cv::IMREAD_COLOR);

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
         input = utility::Camera::getImage();
      }

      if (input.empty())
      {
         return;
      }

      auto &detector = *detectors[detectorIndex];
      auto detectorResult = detector.detect(input);

      std::vector<barcode::api::Result> barcodeDecodingResults;
      std::transform(detectorResult.contours.begin(),
                     detectorResult.contours.end(),
                     std::inserter(barcodeDecodingResults, barcodeDecodingResults.begin()),
                     [&](auto const &descriptor)
                     { 
                        auto result = barcode::api::Decoder::decode(descriptor.id, descriptor.square, descriptor.image, pure);
                        if (dump) 
                        {
                           auto const basePath = std::filesystem::path("out").append(inputPath ? inputPath->stem().string() : "camera");
                           barcode::api::dump(basePath, result, descriptor.image);
                        }
                        return barcode::api::Result::visualize(std::move(result), std::cout); });

      input = detectorResult.visualize(std::move(input), overlayOutputImage);
      auto output = std::reduce(barcodeDecodingResults.begin(),
                                barcodeDecodingResults.end(),
                                std::move(input),
                                [&](auto &&image, barcode::api::Result const &result)
                                {
                                   if (result.level == barcode::api::Level::Decoded)
                                   {
                                      auto const json = uic918::api::Interpreter::interpretPretty(result.payload);
                                      if (json)
                                      {
                                         cv::putText(image, *json, cv::Point(0, 140), cv::FONT_HERSHEY_SIMPLEX, 1., cv::Scalar(0, 0, 255), 2);
                                      }
                                   }
                                   return result.visualize(std::move(image));
                                });
      if (inputAnnotation)
      {
         cv::putText(output, inputAnnotation.value(), cv::Point(0, 70), cv::FONT_HERSHEY_SIMPLEX, 1., cv::Scalar(0, 0, 255), 2);
      }

      utility::Window::show(output); 
   }); // clang-format on
   return 0;
}
