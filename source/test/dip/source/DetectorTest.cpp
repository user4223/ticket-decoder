
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/detector/api/include/Detector.h"

#include "test/support/include/TestSupport.h"

#include <filesystem>

namespace dip::detection::api
{
        TEST(Detector, createAll)
        {
                auto const classifierFile = ::test::support::getExecutableFolderPath() / "etc" / "dip" / "haarcascade_frontalface_default.xml";
                auto const detectors = Detector::createAll(test::support::getContext(), {classifierFile});
                EXPECT_NE(nullptr, detectors.at(DetectorType::NOP_DETECTOR).get());

#ifdef WITH_SQUARE_DETECTOR
                EXPECT_NE(nullptr, detectors.at(DetectorType::SQUARE_DETECTOR).get());
#else
                EXPECT_FALSE(detectors.contains(DetectorType::SQUARE_DETECTOR));
#endif

#ifdef WITH_CLASSIFIER_DETECTOR
                EXPECT_NE(nullptr, detectors.at(DetectorType::CLASSIFIER_DETECTOR).get());
#else
                EXPECT_FALSE(detectors.contains(DetectorType::CLASSIFIER));
#endif
        }

        TEST(Detector, createMinimal)
        {
                auto const detectors = Detector::createAll(test::support::getContext());
                EXPECT_GT(detectors.size(), 0);
        }
}
