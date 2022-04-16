
#include "lib/include/ImageProcessor.h"
#include "lib/include/SquareDetector.h"
#include "lib/include/ClassifierDetector.h"
#include "lib/include/Utility.h"
#include "lib/include/AztecDecoder.h"

#include <opencv2/highgui.hpp>

#include <memory>
#include <iostream>

int main(int argc, char **argv)
{
   auto const name = "Screen";
   cv::namedWindow(name);
   cv::VideoCapture camera(0);
   if (!camera.isOpened())
   {
      std::cout << "Cannot open camera" << std::endl;
      return -1;
   }

   cv::Mat input;
   auto visualizeOriginal = false;
   auto activeDetector = true;
   auto const processor = std::make_unique<ImageProcessor>();
   auto squareDetector = SquareDetector::create();
   auto classifierDetector = ClassifierDetector::create();
   for (int key = cv::waitKey(1); key != 27 /* ESC*/; key = cv::waitKey(1))
   {
      camera >> input;

      auto &detector = Utility::toggleIf(key == 'd', activeDetector) ? *squareDetector : *classifierDetector;
      auto detected = detector.detect(input);

      std::for_each(detected.descriptors.begin(), detected.descriptors.end(), [&](auto &descriptor)
                    { 
                       auto decoder = AztecDecoder::create(descriptor.image);
                       if (!decoder->detect()) { return; }
                       descriptor.level = ContourDescriptor::Level::Detected;
                       std::cout << "." << std::flush;

                       auto const [success, result] = decoder->decode(); 
                       if (!success) { return; }                        
                       descriptor.level = ContourDescriptor::Level::Decoded; 
                       std::cout << "+" << std::flush;
                       
                       /* TODO Do some thing with result data */ });

      auto output = detected.visualize(Utility::toggleIf(key == 'v', visualizeOriginal) ? input : detected.input);
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
