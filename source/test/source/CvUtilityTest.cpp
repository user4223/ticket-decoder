
#include <gtest/gtest.h>

#include "lib/include/CvUtility.h"

TEST(Splitter, invalid)
{
  EXPECT_THROW(Splitter::getPart(cv::Size(500, 1000), 2, 0), std::runtime_error);
  EXPECT_THROW(Splitter::getPart(cv::Size(500, 1000), 4, 0), std::runtime_error);
  EXPECT_THROW(Splitter::getPart(cv::Size(500, 1000), 0, 1), std::runtime_error);
  EXPECT_THROW(Splitter::getPart(cv::Size(500, 1000), 2, 3), std::runtime_error);
  EXPECT_THROW(Splitter::getPart(cv::Size(500, 1000), 4, 5), std::runtime_error);
}

TEST(Splitter, getParts2)
{
  EXPECT_EQ(cv::Rect(0, 0, 500, 500), Splitter::getPart(cv::Size(500, 1000), 2, 1));
  EXPECT_EQ(cv::Rect(0, 500, 500, 500), Splitter::getPart(cv::Size(500, 1000), 2, 2));
  EXPECT_EQ(cv::Rect(0, 0, 500, 500), Splitter::getPart(cv::Size(1000, 500), 2, 1));
  EXPECT_EQ(cv::Rect(500, 0, 500, 500), Splitter::getPart(cv::Size(1000, 500), 2, 2));
}

TEST(Splitter, getParts4)
{
  EXPECT_EQ(cv::Rect(0, 0, 250, 500), Splitter::getPart(cv::Size(500, 1000), 4, 1));
  EXPECT_EQ(cv::Rect(250, 0, 250, 500), Splitter::getPart(cv::Size(500, 1000), 4, 2));
  EXPECT_EQ(cv::Rect(0, 500, 250, 500), Splitter::getPart(cv::Size(500, 1000), 4, 3));
  EXPECT_EQ(cv::Rect(250, 500, 250, 500), Splitter::getPart(cv::Size(500, 1000), 4, 4));

  EXPECT_EQ(cv::Rect(0, 0, 500, 250), Splitter::getPart(cv::Size(1000, 500), 4, 1));
  EXPECT_EQ(cv::Rect(500, 0, 500, 250), Splitter::getPart(cv::Size(1000, 500), 4, 2));
  EXPECT_EQ(cv::Rect(0, 250, 500, 250), Splitter::getPart(cv::Size(1000, 500), 4, 3));
  EXPECT_EQ(cv::Rect(500, 250, 500, 250), Splitter::getPart(cv::Size(1000, 500), 4, 4));
}