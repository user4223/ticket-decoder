
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "test/support/include/Loader.h"

#include "lib/io/image/include/ImageReader.h"

namespace io::image
{
    static auto loggerFactory = utility::LoggerFactory::createLazy(true);
    static auto const x = 1050;
    static auto const y = 1485;

    TEST(ImageReader, readColoredJpg)
    {
        auto reader = ImageReader(loggerFactory, api::ReaderOptions{{}, {}, false});
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.jpg").getImage();
        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(3, real.channels());
        EXPECT_EQ(cv::Vec3b(0, 0, 0), real.at<cv::Vec3b>(100, x / 2));         // black
        EXPECT_EQ(cv::Vec3b(0, 0, 254), real.at<cv::Vec3b>(y / 2, 100));       // red
        EXPECT_EQ(cv::Vec3b(255, 255, 255), real.at<cv::Vec3b>(y / 2, x / 2)); // white
        EXPECT_EQ(cv::Vec3b(254, 0, 0), real.at<cv::Vec3b>(y / 2, x - 100));   // blue
        EXPECT_EQ(cv::Vec3b(1, 255, 0), real.at<cv::Vec3b>(y - 100, x / 2));   // green
    }

    TEST(ImageReader, readGrayJpg)
    {
        auto reader = ImageReader(loggerFactory, api::ReaderOptions{{}, {}, true});
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.jpg").getImage();
        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(1, real.channels());
        EXPECT_EQ(0, real.at<std::uint8_t>(100, x / 2));       // black
        EXPECT_EQ(76, real.at<std::uint8_t>(y / 2, 100));      // red
        EXPECT_EQ(255, real.at<std::uint8_t>(y / 2, x / 2));   // white
        EXPECT_EQ(29, real.at<std::uint8_t>(y / 2, x - 100));  // blue
        EXPECT_EQ(150, real.at<std::uint8_t>(y - 100, x / 2)); // green
    }

    TEST(ImageReader, readColoredPng)
    {
        auto reader = ImageReader(loggerFactory, api::ReaderOptions{{}, {}, false});
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.png").getImage();
        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(3, real.channels());
        EXPECT_EQ(cv::Vec3b(0, 0, 0), real.at<cv::Vec3b>(100, x / 2));         // black
        EXPECT_EQ(cv::Vec3b(0, 0, 255), real.at<cv::Vec3b>(y / 2, 100));       // red
        EXPECT_EQ(cv::Vec3b(255, 255, 255), real.at<cv::Vec3b>(y / 2, x / 2)); // white
        EXPECT_EQ(cv::Vec3b(255, 0, 0), real.at<cv::Vec3b>(y / 2, x - 100));   // blue
        EXPECT_EQ(cv::Vec3b(0, 255, 0), real.at<cv::Vec3b>(y - 100, x / 2));   // green
    }

    TEST(ImageReader, readGrayPng)
    {
        auto reader = ImageReader(loggerFactory, api::ReaderOptions{{}, {}, true});
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.png").getImage();
        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(1, real.channels());
        EXPECT_EQ(0, real.at<std::uint8_t>(100, x / 2));       // black
        EXPECT_EQ(76, real.at<std::uint8_t>(y / 2, 100));      // red
        EXPECT_EQ(255, real.at<std::uint8_t>(y / 2, x / 2));   // white
        EXPECT_EQ(29, real.at<std::uint8_t>(y / 2, x - 100));  // blue
        EXPECT_EQ(149, real.at<std::uint8_t>(y - 100, x / 2)); // green
    }
}
