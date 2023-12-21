#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"

namespace io::api
{
    TEST(SinkManager, sourceAndSinkFile)
    {
        auto sink = SinkManager::create()
                        .fromSource("input/file.pdf")
                        .deriveDestination("out/")
                        .build();
    }
}
