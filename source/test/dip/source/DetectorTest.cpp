
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/detector/api/include/Detector.h"

#include "test/support/include/TestSupport.h"

#include <filesystem>

namespace dip::detection::api
{
    TEST(Detector, createAll)
    {
#ifdef WITH_CLASSIFIER_DETECTOR
        auto const classifierFile = ::test::support::getExecutableFolderPath() / "etc" / "dip" / "haarcascade_frontalface_default.xml";
        auto const detectors = Detector::createAll(test::support::getContext(), {classifierFile});
        EXPECT_EQ(detectors.size(), 3);
        EXPECT_NE(nullptr, detectors.at(DetectorType::NOP_FORWARDER).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::SQUARE_DETECTOR).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::CLASSIFIER).get());
#else
        auto const detectors = Detector::createAll(test::support::getContext());
        EXPECT_EQ(detectors.size(), 2);
        EXPECT_NE(nullptr, detectors.at(DetectorType::NOP_FORWARDER).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::SQUARE_DETECTOR).get());
        EXPECT_FALSE(detectors.contains(DetectorType::CLASSIFIER));
#endif
    }

    TEST(Detector, createMinimal)
    {
        auto const detectors = Detector::createAll(test::support::getContext());
        EXPECT_EQ(detectors.size(), 2);
        EXPECT_NE(nullptr, detectors.at(DetectorType::NOP_FORWARDER).get());
        EXPECT_NE(nullptr, detectors.at(DetectorType::SQUARE_DETECTOR).get());
    }
}
