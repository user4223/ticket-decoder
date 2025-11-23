// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/input/api/include/InputElement.h"

#include <fstream>

namespace input::api
{
    TEST(InputElement, removeLeadingRelativeParts)
    {
        EXPECT_EQ("", InputElement::removeLeadingRelativeParts("").string());
        EXPECT_EQ("", InputElement::removeLeadingRelativeParts(".").string());
        EXPECT_EQ("", InputElement::removeLeadingRelativeParts("./").string());
        EXPECT_EQ("", InputElement::removeLeadingRelativeParts("./../").string());
        EXPECT_EQ("foo.png", InputElement::removeLeadingRelativeParts("foo.png").string());
        EXPECT_EQ("bar/foo.png", InputElement::removeLeadingRelativeParts("bar/foo.png").string());
        EXPECT_EQ("bar/foo.png", InputElement::removeLeadingRelativeParts("../bar/foo.png").string());
        EXPECT_EQ("bar/foo.png", InputElement::removeLeadingRelativeParts("../../bar/foo.png").string());
        EXPECT_EQ("bar/foo.png", InputElement::removeLeadingRelativeParts("./bar/foo.png").string());
        EXPECT_EQ("bar/foo.png", InputElement::removeLeadingRelativeParts(".././bar/foo.png").string());
        EXPECT_EQ("bar/foo.png", InputElement::removeLeadingRelativeParts("././bar/foo.png").string());
        EXPECT_EQ("bar/../foo.png", InputElement::removeLeadingRelativeParts("../../bar/../foo.png").string());
    }

    TEST(InputElement, createRelativeUniquePathFromRelative)
    {
        EXPECT_EQ("", InputElement::createRelativeUniquePath("", std::nullopt));
        EXPECT_EQ("", InputElement::createRelativeUniquePath("./", std::nullopt));
        EXPECT_EQ("item.bla", InputElement::createRelativeUniquePath("item.bla", std::nullopt));
        EXPECT_EQ("foo/bar/item.bla", InputElement::createRelativeUniquePath(std::filesystem::path("foo") / "bar" / "item.bla", std::nullopt));
        EXPECT_EQ("item.bla", InputElement::createRelativeUniquePath(std::filesystem::path("foo") / ".." / "item.bla", std::nullopt));
        EXPECT_EQ("item.bla", InputElement::createRelativeUniquePath(std::filesystem::path("foo") / ".." / "." / ".." / "item.bla", std::nullopt));
        EXPECT_EQ("foo/bar/item.bla_23", InputElement::createRelativeUniquePath(std::filesystem::path("foo") / "bar" / "item.bla", 23));
    }

    TEST(InputElement, createRelativeUniquePathFromAbsolute)
    {
        EXPECT_EQ("", InputElement::createRelativeUniquePath(std::filesystem::current_path(), std::nullopt));
        EXPECT_EQ("item.bla", InputElement::createRelativeUniquePath(std::filesystem::current_path() / "item.bla", std::nullopt));
        EXPECT_EQ("foo/bar/item.bla", InputElement::createRelativeUniquePath(std::filesystem::current_path() / "foo" / "bar" / "item.bla", std::nullopt));
        EXPECT_EQ("item.bla", InputElement::createRelativeUniquePath(std::filesystem::current_path() / "foo" / ".." / "item.bla", std::nullopt));
        EXPECT_EQ("item.bla", InputElement::createRelativeUniquePath(std::filesystem::current_path() / "foo" / ".." / "." / ".." / "item.bla", std::nullopt));
        EXPECT_EQ("item.bla_42", InputElement::createRelativeUniquePath(std::filesystem::current_path() / "item.bla", 42));
    }

    TEST(InputElement, empty)
    {
        auto element = InputElement::empty();
        EXPECT_FALSE(element.isValid());
        EXPECT_TRUE(element.isVirtual());
        EXPECT_TRUE(element.getImage().empty());
        EXPECT_EQ("empty", element.getAnnotation());
        EXPECT_EQ(std::nullopt, element.getPath());
        EXPECT_EQ(std::filesystem::path("empty"), element.getUniquePath());
    }

    TEST(InputElement, fromCamera)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromCamera(cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_TRUE(element.isVirtual());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ(InputElement::CAMERA_ANNOTATION, element.getAnnotation());
        EXPECT_EQ(std::nullopt, element.getPath());
        EXPECT_EQ(std::filesystem::path(InputElement::CAMERA_ANNOTATION), element.getUniquePath());
    }

    TEST(InputElement, fromFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("foo.jpg", cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_FALSE(element.isVirtual());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo.jpg", element.getAnnotation());
        EXPECT_EQ("foo.jpg", element.getPath().value_or("broken"));
        EXPECT_EQ("foo.jpg", element.getUniquePath());
    }

    TEST(InputElement, fromMultiImageFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("foo.pdf", 2, cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_FALSE(element.isVirtual());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo.pdf[2]", element.getAnnotation());
        EXPECT_EQ("foo.pdf", element.getPath().value_or("broken"));
        EXPECT_EQ("foo.pdf_2", element.getUniquePath());
    }

    TEST(InputElement, fromRelativeFile)
    {
        char *data = (char *)"a";
        auto element = InputElement::fromFile("some/path/foo.jpg", cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_FALSE(element.isVirtual());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo.jpg", element.getAnnotation());
        EXPECT_EQ("some/path/foo.jpg", element.getPath().value_or("broken"));
        EXPECT_EQ("some/path/foo.jpg", element.getUniquePath());
    }

    TEST(InputElement, fromAbsoluteFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("path");
        std::ofstream("path/foo.jpg").close();
        auto const path = std::filesystem::current_path() / "path" / "foo.jpg";
        EXPECT_TRUE(std::filesystem::exists(path));
        char *data = (char *)"a";
        auto element = InputElement::fromFile(path, cv::Mat(1, 1, CV_8UC1, data));
        EXPECT_TRUE(element.isValid());
        EXPECT_FALSE(element.isVirtual());
        EXPECT_EQ(1, element.getImage().rows);
        EXPECT_EQ(1, element.getImage().cols);
        EXPECT_EQ("foo.jpg", element.getAnnotation());
        EXPECT_EQ(path, element.getPath().value_or("broken"));
        EXPECT_EQ("path/foo.jpg", element.getUniquePath());
    }
}
