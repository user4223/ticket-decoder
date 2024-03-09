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
        EXPECT_EQ("out/folder/image.png", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/folder/image.png.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceAndDestinationDirectory2)
    {
        auto manager = SinkManager::create()
                           .useSource("input/folder/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/image.png", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/image.png.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        auto manager = SinkManager::create()
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image.png", manager.deriveSinkPath("folder/image.png"));
        EXPECT_EQ("out/folder/image.png.raw", manager.deriveSinkPath("folder/image.png", ".raw"));
    }
}
