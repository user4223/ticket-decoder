#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"

namespace io::api
{
    TEST(SinkManager, sourceAndSinkFile)
    {
        auto manager = SinkManager::create()
                           .useSource("input/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image", manager.deriveSinkPath("input/folder/image.png"));
    }
}
