
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

   // camera.set(cv::CAP_PROP_FRAME_WIDTH, 960);
   // camera.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
   std::cout << camera.get(cv::CAP_PROP_FRAME_WIDTH)
             << "x" << camera.get(cv::CAP_PROP_FRAME_HEIGHT)
             << " " << camera.get(cv::CAP_PROP_ZOOM) << std::endl;

   auto quit = false, showOriginalImage = true, useContourDetector = true, dump = false, copyDetected = true;
   auto parameters = Detector::Parameters{};
   parameters.a = 13;
   parameters.b = 1;

   auto squareDetector = SquareDetector::create(parameters);
   auto classifierDetector = ClassifierDetector::create();

   auto keyMapper = KeyMapper({
       {'a', [&]()
        { return "a: " + std::to_string(parameters.a += 2); }},
       {'A', [&]()
        { return "a: " + std::to_string(parameters.a -= 2); }},
       {'b', [&]()
        { return "b: " + std::to_string(parameters.b++); }},
       {'B', [&]()
        { return "b: " + std::to_string(parameters.b--); }},
       {'d', [&]()
        { return "d: " + std::to_string(useContourDetector = !useContourDetector); }},
       {'v', [&]()
        { return "v: " + std::to_string(showOriginalImage = !showOriginalImage); }},
       {'c', [&]()
        { return "c: " + std::to_string(copyDetected = !copyDetected); }},
       {' ', [&]()
        { dump = true; return "dump"; }},
       {27, [&]()
        { quit = true; return "quit"; }},
   });
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
      auto detected = detector.detect(input);

      std::for_each(detected.descriptors.begin(), detected.descriptors.end(), [&](auto &descriptor)
                    { 
                       if (descriptor.image.empty()) { return; }

                       auto decoder = AztecDecoder::create(descriptor.image);
                       if (!decoder->detect()) { return; }
                       descriptor.level = ContourDescriptor::Level::Detected;
                       std::cout << "." << std::flush;

                       auto const [success, result] = decoder->decode(); 
                       if (!success) { return; }                        
                       descriptor.level = ContourDescriptor::Level::Decoded; 
                       std::cout << "+" << std::flush;
                       
                       /*cv::imwrite(Utility::uniqueFilename("out", "jpg"), descriptor.image);*/ });

      auto const output = detected.visualize(showOriginalImage ? input : detected.input, copyDetected);
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
