
#include "lib/include/ImageProcessor.h"
#include "lib/include/SquareDetector.h"
#include "lib/include/ContourDetectorParameters.h"
#include "lib/include/ClassifierDetector.h"
#include "lib/include/Utility.h"
#include "lib/include/DeviceController.h"
#include "lib/include/KeyMapper.h"

#include "lib/aztec/include/AztecDecoder.h"

#include <opencv2/highgui.hpp>

#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <numeric>

int main(int argc, char **argv)
{
   // auto controller = DeviceController{};

   auto const name = "Screen";
   cv::namedWindow(name);
   cv::VideoCapture camera(0);
   if (!camera.isOpened())
   {
      std::cout << "Cannot open camera" << std::endl;
      return -1;
   }

   std::cout << camera.get(cv::CAP_PROP_FRAME_WIDTH)
             << "x" << camera.get(cv::CAP_PROP_FRAME_HEIGHT)
             << " " << camera.get(cv::CAP_PROP_ZOOM) << std::endl;

   auto quit = false, dump = false, useContourDetector = true;
   auto parameters = ContourDetectorParameters{};

   auto squareDetector = SquareDetector::create(parameters);
   auto classifierDetector = ClassifierDetector::create();
   auto keyMapper = KeyMapper( // clang-format off
   {    
       {'i', [&](){ return "i: " + std::to_string(++parameters.imageProcessingDebugStep); }},
       {'I', [&](){ return "I: " + std::to_string(Utility::safeDecrement(parameters.imageProcessingDebugStep)); }},
       {'c', [&](){ return "c: " + std::to_string(++parameters.contourDetectorDebugStep); }},
       {'C', [&](){ return "C: " + std::to_string(Utility::safeDecrement(parameters.contourDetectorDebugStep)); }},
       {'d', [&](){ return "d: " + std::to_string(useContourDetector = !useContourDetector); }},
       {' ', [&](){ dump = true; return "dump"; }},
       {27,  [&](){ quit = true; return "quit"; }},
   }); // clang-format on

   for (int key = cv::waitKey(1); !quit; key = cv::waitKey(1))
   {
      keyMapper.handle(key, std::cout);

      cv::Mat input;
      camera >> input;
      // auto input = cv::imread(std::string("../../images/") + "Muster_918-3_Quer-durchs-Land-Ticket.png", cv::IMREAD_COLOR);
      if (input.empty())
      {
         continue;
      }

      auto &contourDetector = useContourDetector ? *squareDetector : *classifierDetector;
      auto contourDetectorResult = contourDetector.detect(input);

      std::vector<BarcodeDecodingResult> barcodeDecodingResults;
      std::transform(contourDetectorResult.contours.begin(),
                     contourDetectorResult.contours.end(),
                     std::inserter(barcodeDecodingResults, barcodeDecodingResults.begin()),
                     [&](auto const &descriptor)
                     { return AztecDecoder::decode(descriptor, true); });

      auto output = std::reduce(barcodeDecodingResults.begin(),
                                barcodeDecodingResults.end(),
                                contourDetectorResult.visualize(std::move(input)),
                                [](auto &&image, auto const &result)
                                { return result.visualize(std::move(image)); });

      cv::imshow(name, output);

      if (dump)
      {
         dump = false;
         cv::imwrite(Utility::uniqueFilename("out", "jpg"), output);
      }
   }
   cv::destroyAllWindows();
   return 0;
}
