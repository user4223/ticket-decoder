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
    }

    TEST(SinkManager, sourceAndDestinationDirectory2)
    {
        auto manager = SinkManager::create()
                           .useSource("input/folder/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/image", manager.deriveSinkPath("input/folder/image.png"));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        auto manager = SinkManager::create()
                           .useSource("input/folder/image.png")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image", manager.deriveSinkPath("input/folder/image.png"));
    }
}
