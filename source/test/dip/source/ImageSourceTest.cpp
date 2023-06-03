
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/dip/utility/include/ImageSource.h"

namespace dip::utility
{

  TEST(ImageSource, splitStringToPair1)
  {
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair(""));
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("blubber"));
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("00"));
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("11"));
  }

  TEST(ImageSource, splitStringToPair2)
  {
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("20"));
    EXPECT_EQ(std::make_pair(2u, 1u), splitStringToPair("21"));
    EXPECT_EQ(std::make_pair(2u, 2u), splitStringToPair("22"));
  }

  TEST(ImageSource, splitStringToPair4)
  {
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("40"));
    EXPECT_EQ(std::make_pair(4u, 1u), splitStringToPair("41"));
    EXPECT_EQ(std::make_pair(4u, 2u), splitStringToPair("42"));
    EXPECT_EQ(std::make_pair(4u, 3u), splitStringToPair("43"));
    EXPECT_EQ(std::make_pair(4u, 4u), splitStringToPair("44"));
  }

  TEST(ImageSource, splitPairToMap1)
  {
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({0, 0}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({1, 1}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({3, 2}));
  }

  TEST(ImageSource, splitPairToMap2)
  {
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({2, 0}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 1u}, {4u, 0u}}), splitPairToMap({2, 1}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 2u}, {4u, 0u}}), splitPairToMap({2, 2}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 2u}, {4u, 0u}}), splitPairToMap({2, 3}));
  }

  TEST(ImageSource, splitPairToMap4)
  {
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({4, 0}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 1u}}), splitPairToMap({4, 1}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 2u}}), splitPairToMap({4, 2}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 3u}}), splitPairToMap({4, 3}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 4u}}), splitPairToMap({4, 4}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 4u}}), splitPairToMap({4, 5}));
  }
}
