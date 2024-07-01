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
        EXPECT_EQ(std::nullopt, element.getPath());
        EXPECT_EQ(std::nullopt, element.getUniquePath());
    }

    TEST(InputElement, fromCamera)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromCamera(cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("Camera", element.getAnnotation());
        EXPECT_EQ(std::nullopt, element.getPath());
        EXPECT_EQ(std::nullopt, element.getUniquePath());
    }

    TEST(InputElement, fromFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("foo.jpg", cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo.jpg", element.getAnnotation());
        EXPECT_EQ("foo.jpg", element.getPath().value_or("broken"));
        EXPECT_EQ("foo.jpg", element.getUniquePath().value_or("broken"));
    }

    TEST(InputElement, fromMultiImageFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("foo.pdf", 2, cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo.pdf[2]", element.getAnnotation());
        EXPECT_EQ("foo.pdf", element.getPath().value_or("broken"));
        EXPECT_EQ("foo.pdf_2", element.getUniquePath().value_or("broken"));
    }

    TEST(InputElement, fromRelativeFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("some/path/foo.jpg", cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("some/path/foo.jpg", element.getAnnotation());
        EXPECT_EQ("some/path/foo.jpg", element.getPath().value_or("broken"));
        EXPECT_EQ("some/path/foo.jpg", element.getUniquePath().value_or("broken"));
    }
}
