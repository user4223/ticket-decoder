#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/SourceManager.h"

namespace io::api
{
    TEST(SourceManager, empty)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto sourceManager = SourceManager::create(loggerFactory, LoadResult(std::vector<InputElement>{}));
        EXPECT_FALSE(sourceManager.isCameraEnabled());
        EXPECT_EQ("empty", sourceManager.next());
        EXPECT_EQ(std::nullopt, sourceManager.get());
        sourceManager.next();
        EXPECT_EQ("empty", sourceManager.next());
        EXPECT_EQ(std::nullopt, sourceManager.get());
        sourceManager.previous();
        EXPECT_EQ("empty", sourceManager.next());
        EXPECT_EQ(std::nullopt, sourceManager.get());
    }

    TEST(SourceManager, initial)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        char *first = (char *)"a";
        char *second = (char *)"b";
        auto sourceManager = SourceManager::create(loggerFactory, LoadResult({InputElement::fromFile("first", cv::Mat{1, 1, CV_8UC1, first}),
                                                                              InputElement::fromFile("second", cv::Mat{1, 1, CV_8UC1, second})}));
        EXPECT_FALSE(sourceManager.isCameraEnabled());
        sourceManager.toggleCamera();
        EXPECT_TRUE(sourceManager.isCameraEnabled());
        sourceManager.toggleCamera();
        EXPECT_FALSE(sourceManager.isCameraEnabled());

        EXPECT_EQ("first", sourceManager.get()->getAnnotation());
        sourceManager.previous();
        EXPECT_EQ("first", sourceManager.get()->getAnnotation());
        sourceManager.next();
        EXPECT_EQ("second", sourceManager.get()->getAnnotation());
        sourceManager.previous();
        EXPECT_EQ("first", sourceManager.get()->getAnnotation());
        sourceManager.next();
        EXPECT_EQ("second", sourceManager.get()->getAnnotation());
        sourceManager.next();
        EXPECT_EQ("second", sourceManager.get()->getAnnotation());
        sourceManager.previous();
        EXPECT_EQ("first", sourceManager.get()->getAnnotation());
    }
}
