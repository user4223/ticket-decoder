#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/InputElement.h"

namespace io::api
{
    TEST(InputElement, empty)
    {
        auto element = InputElement::empty();
        EXPECT_FALSE(element.isValid());
        EXPECT_TRUE(element.getImage().empty());
        EXPECT_EQ("empty", element.getAnnotation());
    }

    TEST(InputElement, fromCamera)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromCamera(cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("camera", element.getAnnotation());
    }

    TEST(InputElement, fromFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("foo", cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo", element.getAnnotation());
    }
}
