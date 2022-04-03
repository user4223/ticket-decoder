
#include "lib/include/PreProcessor.h"
#include "lib/include/Detector.h"

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
   auto const processor = std::make_unique<PreProcessor>();
   auto const detector = std::make_unique<Detector>();
   while (cv::waitKey(1) != 27)
   {
      camera >> input;
      auto preProcessed = processor->process(input);
      auto detected = detector->detect(preProcessed);

      cv::imshow(name, detected.visualize(preProcessed));
   }

   return 0;
}
