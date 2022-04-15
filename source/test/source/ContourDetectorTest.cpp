
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <opencv2/core.hpp>

#include "lib/include/ContourDetector.h"

TEST(NormalizePointOrder, noRectangle)
{
  EXPECT_THROW(ContourDetector::normalizePointOrder(std::vector<cv::Point>{{0, 1}, {0, 0}}), std::runtime_error);
}

TEST(NormalizePointOrder, alreadyOrdered)
{
  auto const result = ContourDetector::normalizePointOrder(std::vector<cv::Point>{{0, 1}, {1, 1}, {1, 0}, {0, 0}});
  EXPECT_EQ(cv::Point(0, 1), result[0]);
  EXPECT_EQ(cv::Point(1, 1), result[1]);
  EXPECT_EQ(cv::Point(1, 0), result[2]);
  EXPECT_EQ(cv::Point(0, 0), result[3]);
}

TEST(NormalizePointOrder, unOrdered)
{
  auto const result = ContourDetector::normalizePointOrder(std::vector<cv::Point>{{1, 1}, {0, 1}, {0, 0}, {1, 0}});
  EXPECT_EQ(cv::Point(0, 1), result[0]);
  EXPECT_EQ(cv::Point(1, 1), result[1]);
  EXPECT_EQ(cv::Point(1, 0), result[2]);
  EXPECT_EQ(cv::Point(0, 0), result[3]);
}
