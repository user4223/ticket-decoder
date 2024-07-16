#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    TEST(SinkManager, sourceAndDestinationDirectory1)
    {
        auto manager = SinkManager::create(loggerFactory)
                           .useSource("input/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image.png", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/folder/image.png.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceAndDestinationDirectory2)
    {
        auto manager = SinkManager::create(loggerFactory)
                           .useSource("input/folder/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/image.png", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/image.png.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image.png", manager.deriveSinkPath("folder/image.png"));
        EXPECT_EQ("out/folder/image.png.raw", manager.deriveSinkPath("folder/image.png", ".raw"));
    }

    TEST(SinkManager, cameraSource)
    {
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .useSource("images/")
                           .build();
        EXPECT_EQ("out/Camera", manager.deriveSinkPath("Camera"));
    }
}
