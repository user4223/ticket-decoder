
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
   auto decoder = AztecDecoder::create();
   for (int key = cv::waitKey(1); key != 27 /* ESC*/; key = cv::waitKey(1))
   {
      camera >> input;

      auto &detector = Utility::toggleIf(key == 'd', activeDetector) ? *squareDetector : *classifierDetector;
      auto detected = detector.detect(input);

      std::for_each(detected.descriptors.begin(), detected.descriptors.end(), [&](auto &descriptor)
                    { 
                       auto const [success, result] = decoder->detect(descriptor.image); 
                       if (success) 
                       { descriptor.level = ContourDescriptor::Level::Detected; } });

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

// Just backup
//
// auto bullseyeDetector(cv::Mat const &input)
// {
//   auto const y_border = input.rows / 10;
//   for (int y = 0; y < input.rows; ++y)
//   {
//     // detect 6 light regions separated from dark
//     // use binary search from the middle to upper/lower image boundaries
//     auto const x_border = input.cols / 10;
//     auto it = cv::LineIterator(input, cv::Point(x_border, y), cv::Point(input.cols - x_border, y), 4);
//     for (int i = 0; i < it.count; ++i)
//     {
//       auto const value = **it;
//     }
//   }
// }
