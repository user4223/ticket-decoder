// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/input/detail/api/include/ReadResult.h"

namespace input::detail
{

    TEST(ReadResult, isNoMultipart)
    {
        EXPECT_FALSE(ReadResult(cv::Mat{}).isMultiPart());
        EXPECT_FALSE(ReadResult(std::vector<cv::Mat>{cv::Mat{}}).isMultiPart());
    }

    TEST(ReadResult, isMultipart)
    {
        auto result = ReadResult(std::vector<cv::Mat>{cv::Mat{}, cv::Mat{}});
        EXPECT_TRUE(result.isMultiPart());
        EXPECT_THROW(result.getImage(), std::runtime_error);
        EXPECT_EQ(result.getImages().size(), 2);
    }

    TEST(ReadResult, noPart)
    {
        EXPECT_THROW(ReadResult(std::vector<cv::Mat>{}), std::runtime_error);
    }
}
