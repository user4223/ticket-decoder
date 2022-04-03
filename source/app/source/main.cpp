
#include "lib/include/PreProcessor.h"

#include <opencv2/opencv.hpp>

#include <memory>

int main(int argc, char **argv)
{
   cv::namedWindow("Output");

   cv::Mat image;
   cv::VideoCapture camera(0);
   if (!camera.isOpened())
   {
      std::cout << "Cannot open camera" << std::endl;
      return -1;
   }

   auto const processor = std::make_unique<PreProcessor>();
   while (cv::waitKey(1) != 27)
   {
      camera >> image;
      cv::imshow("Output", processor->process(image));
   }

   return 0;
}
