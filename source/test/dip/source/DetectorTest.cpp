
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "lib/dip/detection/api/include/Detector.h"

#include "test/support/include/Loader.h"

#include <filesystem>

namespace dip::detection::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    TEST(Detector, createAll)
    {
        auto const classifierFile = support::Loader::getExecutableFolderPath() / "etc" / "dip" / "haarcascade_frontalface_default.xml";
        auto const detectors = Detector::createAll(loggerFactory, {classifierFile});
        EXPECT_EQ(detectors.size(), 3);
    }
}
