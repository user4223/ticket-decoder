
#include "lib/include/PreProcessor.h"
#include "lib/include/Detector.h"
#include "lib/include/Visualizer.h"

#include <opencv2/opencv.hpp>

#include <memory>

int main(int argc, char **argv)
{
   cv::VideoCapture camera(0);
   if (!camera.isOpened())
   {
      std::cout << "Cannot open camera" << std::endl;
      return -1;
   }

   cv::Mat input;
   auto const visualizer = std::make_unique<Visualizer>();
   auto const processor = std::make_unique<PreProcessor>();
   auto const detector = std::make_unique<Detector>();
   while (cv::waitKey(1) != 27)
   {
      camera >> input;
      auto preProcessed = processor->process(input);
      auto detected = detector->detect(preProcessed);

      visualizer->visualize(preProcessed, detected.contours);
   }

   return 0;
}
