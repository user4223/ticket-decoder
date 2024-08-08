
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "lib/dip/detection/api/include/Detector.h"

#include "test/support/include/Loader.h"

#include <filesystem>

namespace dip::detection::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);
    static auto debugController = ::utility::DebugController{};

    TEST(Detector, createAll)
    {
        auto const classifierFile = support::Loader::getExecutableFolderPath() / "etc" / "dip" / "haarcascade_frontalface_default.xml";
        auto const detectors = Detector::createAll(loggerFactory, debugController, {classifierFile});
        EXPECT_EQ(detectors.size(), 3);
        EXPECT_NE(nullptr, detectors.at(DetectorType::NOP_FORWARDER).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::SQUARE_DETECTOR).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::CLASSIFIER).get());
    }

    TEST(Detector, createMinimal)
    {
        auto const detectors = Detector::createAll(loggerFactory, debugController);
        EXPECT_EQ(detectors.size(), 2);
        EXPECT_NE(nullptr, detectors.at(DetectorType::NOP_FORWARDER).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::SQUARE_DETECTOR).get());
    }
}
