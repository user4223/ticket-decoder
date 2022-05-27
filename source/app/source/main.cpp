
#include "lib/include/ImageProcessor.h"
#include "lib/include/SquareDetector.h"
#include "lib/include/ClassifierDetector.h"
#include "lib/include/Utility.h"
#include "lib/include/AztecDecoder.h"
#include "lib/include/DeviceController.h"
#include "lib/include/KeyMapper.h"

#include <opencv2/highgui.hpp>

#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>

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
   auto parameters = Detector::Parameters{};

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

      auto &detector = useContourDetector ? *squareDetector : *classifierDetector;
      auto result = detector.detect(input);

      std::for_each(result.contours.begin(), result.contours.end(), [&](auto &descriptor)
                    { 
                       auto decoder = AztecDecoder::create(descriptor.image, true);
                       if (Decoder::Level::Detected != decoder->detect()) { return; }
                       std::cout << "." << std::flush;

                       auto const [level, payload] = decoder->decode(); 
                       if (Decoder::Level::Decoded != level) { return; }                        
                       std::cout << "+" << std::flush;
                       
                       //std::ofstream{std::filesystem::path("current.raw"), std::ios::binary}.write((char const*)&(payload[0]), payload.size());
                       //cv::imwrite(Utility::uniqueFilename("out", "jpg"), descriptor.image);
                       /**/ });

      auto const output = result.visualize(input);
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
