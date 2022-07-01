
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/include/ImageProcessor.h"

TEST(ImageProcessor, filter)
{
   auto const descriptor = ImageProcessor::filter(ImageDescriptor::fromImage(cv::Mat(10, 10, CV_8UC1)),
                                                  {[&](ImageDescriptor &&descriptor)
                                                   {
                                                      EXPECT_EQ(descriptor.stepCount, 1);
                                                      descriptor.image.at<std::uint8_t>(5, 5) = 23;
                                                      return std::move(descriptor);
                                                   },
                                                   [&](ImageDescriptor &&descriptor)
                                                   {
                                                      EXPECT_EQ(descriptor.stepCount, 2);
                                                      descriptor.image.at<std::uint8_t>(6, 6) = 42;
                                                      return std::move(descriptor);
                                                   }});
   EXPECT_EQ(descriptor.stepCount, 3);
   EXPECT_EQ(descriptor.image.at<std::uint8_t>(5, 5), 23);
   EXPECT_EQ(descriptor.image.at<std::uint8_t>(6, 6), 42);
}
