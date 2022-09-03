
#include "lib/dip/detection/api/include/SquareDetector.h"
#include "lib/dip/detection/api/include/ClassifierDetector.h"
#include "lib/include/CvUtility.h"

#include "lib/utility/include/KeyMapper.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Utility.h"

#include "lib/dip/filtering/include/Transform.h"

#include "lib/barcode/include/Decoder.h"

#include "lib/uic918/api/include/Interpreter.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <memory>
#include <iostream>
#include <filesystem>
#include <numeric>
#include <algorithm>
#include <map>
#include <sstream>
#include <fstream>

int main(int argc, char **argv)
{
   // auto controller = DeviceController{};

   auto const name = "Screen";
   cv::namedWindow(name);

   auto const paths = utility::scanForImages("../../images/");
   auto parts = std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 2u}};

   auto quit = false, dump = true, useContourDetector = true, pure = false;
   auto inputFileIndex = 1u, rotationDegree = 0u;
   auto inputAnnotation = std::optional<std::string>();
   auto parameters = dip::detection::api::Parameters{7, 17};

   auto squareDetector = dip::detection::api::SquareDetector::create(parameters);
   auto classifierDetector = dip::detection::api::ClassifierDetector::create();
   auto keyMapper = utility::KeyMapper( // clang-format off
   {    
       {'i', [&](){ return "i: " + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "I: " + std::to_string(utility::safeDecrement(parameters.imageProcessingDebugStep)); }},
       {'c', [&](){ return "c: " + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "C: " + std::to_string(utility::safeDecrement(parameters.contourDetectorDebugStep)); }},
       {'f', [&](){ return "f: " + std::to_string(utility::safeIncrement(inputFileIndex, paths.size())); }},
       {'F', [&](){ return "F: " + std::to_string(utility::safeDecrement(inputFileIndex)); }},
       {'r', [&](){ return "r: " + std::to_string(utility::safeIncrement(rotationDegree, 5, 360)); }},
       {'R', [&](){ return "R: " + std::to_string(utility::safeDecrement(rotationDegree, 5)); }},
       {'d', [&](){ return "d: " + std::to_string(useContourDetector = !useContourDetector); }},
       {'p', [&](){ return "p: " + std::to_string(pure = !pure); }},
       {'2', [&](){ return "2: " + std::to_string(utility::rotate(parts.at(2), 2)); }},
       {'4', [&](){ return "4: " + std::to_string(utility::rotate(parts.at(4), 4)); }},
       {' ', [&](){ dump = !dump; return "dump: " + std::to_string(dump); }},
       {27,  [&](){ quit = true; return "quit: " + std::to_string(quit); }},
   }); // clang-format on

   for (int key = cv::waitKey(1); !quit; key = cv::waitKey(1))
   {
      keyMapper.handle(key, std::cout);

      auto const inputPath = inputFileIndex == 0 || paths.empty()
                                 ? std::nullopt
                                 : std::make_optional(paths[std::min((unsigned int)(paths.size()), inputFileIndex) - 1]);

      cv::Mat input;
      if (inputPath)
      {
         Camera::release();
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
         input = Camera::getImage();
      }

      if (input.empty())
      {
         continue;
      }

      auto &contourDetector = useContourDetector ? *squareDetector : *classifierDetector;
      auto contourDetectorResult = contourDetector.detect(input);

      std::vector<barcode::api::BarcodeDecodingResult> barcodeDecodingResults;
      std::transform(contourDetectorResult.contours.begin(),
                     contourDetectorResult.contours.end(),
                     std::inserter(barcodeDecodingResults, barcodeDecodingResults.begin()),
                     [&](auto const &descriptor)
                     { 
                        auto result = barcode::api::Decoder::decode(descriptor.id, descriptor.square, descriptor.image, pure);
                        if (dump) 
                        {
                           auto const outputPath = std::filesystem::path("out")
                                 .append((inputPath ? inputPath->stem().string() : "camera")).string();
                           if (result.level == barcode::api::BarcodeDecodingLevel::Decoded)
                           {
                              std::ofstream{outputPath + ".raw", std::ios::binary}
                                 .write((char const *)&(result.payload[0]), result.payload.size());
                           } 
                           if (!descriptor.image.empty()) 
                           {
                              auto imageOutputPath = outputPath;
                              switch (result.level) 
                              {
                                 case barcode::api::BarcodeDecodingLevel::Decoded: imageOutputPath += "_decoded"; break;
                                 case barcode::api::BarcodeDecodingLevel::Detected: imageOutputPath += "_detected"; break;
                                 default: imageOutputPath += "_failed"; break;
                              }
                              cv::imwrite(imageOutputPath + ".jpg", descriptor.image);
                           }
                        }
                        return barcode::api::BarcodeDecodingResult::visualize(std::move(result), std::cout); });

      input = contourDetectorResult.visualize(std::move(input));
      auto output = std::reduce(barcodeDecodingResults.begin(),
                                barcodeDecodingResults.end(),
                                std::move(input),
                                [](auto &&image, barcode::api::BarcodeDecodingResult const &result)
                                {
                                   if (result.level == barcode::api::BarcodeDecodingLevel::Decoded)
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

      cv::imshow(name, output);
   }
   cv::destroyAllWindows();
   return 0;
}
