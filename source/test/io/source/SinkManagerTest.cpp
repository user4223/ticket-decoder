#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"

namespace io::api
{
    TEST(SinkManager, sourceAndDestinationDirectory1)
    {
        auto manager = SinkManager::create()
                           .useSource("input/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/folder/image.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceAndDestinationDirectory2)
    {
        auto manager = SinkManager::create()
                           .useSource("input/folder/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/image", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/image.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        auto manager = SinkManager::create()
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image", manager.deriveSinkPath("folder/image.png"));
        EXPECT_EQ("out/folder/image.raw", manager.deriveSinkPath("folder/image.png", ".raw"));
    }

    TEST(Sink, writePaths)
    {
        auto sink = SinkManager::create()
                        .useDestination("out/")
                        .build()
                        .get(InputElement::fromFile("folder/document.pdf", cv::Mat{}));
        EXPECT_EQ("out/folder/document.png", sink.write(cv::Mat{}));
        EXPECT_EQ("out/folder/document.json", sink.write(std::string{}));
        EXPECT_EQ("out/folder/document.raw", sink.write(std::vector<uint8_t>{}));
    }
}
