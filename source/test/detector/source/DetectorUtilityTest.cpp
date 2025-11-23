// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <opencv2/core.hpp>

#include "lib/detector/detail/common/include/DetectorUtility.h"

namespace detector::detail
{
  TEST(NormalizePointOrder, noRectangle)
  {
    EXPECT_THROW(normalizePointOrder(std::vector<cv::Point>{{0, 1}, {0, 0}}), std::runtime_error);
    EXPECT_THROW(normalizePointOrder(std::vector<cv::Point>{{0, 1}, {0, 0}, {1, 1}, {0, 0}, {1, 1}}), std::runtime_error);
  }

  TEST(NormalizePointOrder, alreadyOrdered)
  {
    auto const result = normalizePointOrder(std::vector<cv::Point>{{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    EXPECT_EQ(cv::Point(0, 0), result[0]);
    EXPECT_EQ(cv::Point(1, 0), result[1]);
    EXPECT_EQ(cv::Point(1, 1), result[2]);
    EXPECT_EQ(cv::Point(0, 1), result[3]);
  }

  TEST(NormalizePointOrder, unOrdered)
  {
    auto const result = normalizePointOrder(std::vector<cv::Point>{{1, 1}, {0, 1}, {0, 0}, {1, 0}});
    EXPECT_EQ(cv::Point(0, 0), result[0]);
    EXPECT_EQ(cv::Point(1, 0), result[1]);
    EXPECT_EQ(cv::Point(1, 1), result[2]);
    EXPECT_EQ(cv::Point(0, 1), result[3]);
  }
}
