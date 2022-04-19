
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
   std::cout << camera.get(cv::CAP_PROP_FRAME_WIDTH) << "x" << camera.get(cv::CAP_PROP_FRAME_HEIGHT)
             << " " << camera.get(cv::CAP_PROP_ZOOM) << std::endl;

   cv::Mat input;
   auto showOriginalImage = false;
   auto useContourDetector = true;
   auto parameters = Detector::Parameters{};
   parameters.a = 13;
   parameters.b = 1;

   auto squareDetector = SquareDetector::create(parameters);
   auto classifierDetector = ClassifierDetector::create();

   auto keyMapper = KeyMapper({
       {'a', [&]()
        { parameters.a += 2; }},
       {'A', [&]()
        { parameters.a -= 2; }},
       {'b', [&]()
        { parameters.b++; }},
       {'B', [&]()
        { parameters.b--; }},
       {'d', [&]()
        { useContourDetector = !useContourDetector; }},
       {'v', [&]()
        { showOriginalImage = !showOriginalImage; }},
   });
   for (int key = cv::waitKey(1); key != 27 /* ESC*/; key = cv::waitKey(1))
   {
      if (keyMapper.handle(key))
      {
         std::cout << parameters.toString() << std::endl;
      }

      camera >> input;
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
                       
                       cv::imwrite(Utility::uniqueFilename("out", "jpg"), descriptor.image); });

      auto output = detected.visualize(showOriginalImage ? input : detected.input);
      cv::imshow(name, output);

      if (key == ' ')
      {
         auto const file = Utility::uniqueFilename("out", "jpg");
         std::cout << "Saving file: " << file << std::endl;
         cv::imwrite(file, output);
      }
   }
   cv::destroyAllWindows();
   return 0;
}
