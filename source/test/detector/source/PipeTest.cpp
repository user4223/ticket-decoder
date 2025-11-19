
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/detector/detail/common/include/FilterPipe.h"

namespace detector::detail
{

   TEST(Pipe, filter)
   {
      auto const descriptor = FilterPipe::filter(FilterPipeDescriptor::fromImage(cv::Mat(10, 10, CV_8UC1)),
                                                 {[&](FilterPipeDescriptor &&descriptor)
                                                  {
                                                     EXPECT_EQ(descriptor.stepCount, 1);
                                                     descriptor.image.at<std::uint8_t>(5, 5) = 23;
                                                     return std::move(descriptor);
                                                  },
                                                  [&](FilterPipeDescriptor &&descriptor)
                                                  {
                                                     EXPECT_EQ(descriptor.stepCount, 2);
                                                     descriptor.image.at<std::uint8_t>(6, 6) = 42;
                                                     return std::move(descriptor);
                                                  }});
      EXPECT_EQ(descriptor.stepCount, 3);
      EXPECT_EQ(descriptor.image.at<std::uint8_t>(5, 5), 23);
      EXPECT_EQ(descriptor.image.at<std::uint8_t>(6, 6), 42);
   }
}
