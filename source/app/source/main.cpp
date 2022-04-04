
#include "lib/include/ImageProcessor.h"
#include "lib/include/Detector.h"
#include "lib/include/Utility.h"

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
   auto const processor = std::make_unique<ImageProcessor>();
   auto const detector = std::make_unique<Detector>();
   for (int key = cv::waitKey(1); key != 27; key = cv::waitKey(1))
   {
      camera >> input;
      auto preProcessed = processor->preProcess(input);

      if (key == 32)
      {
         auto const file = Utility::uniqueFilename("out", "jpg");
         std::cout << "Saving file: " << file << std::endl;
         cv::imwrite(file, preProcessed);
      }

      auto detected = detector->detect(preProcessed);

      cv::imshow(name, detected.visualize(preProcessed));
   }

   return 0;
}
