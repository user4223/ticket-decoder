
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "test/support/include/Loader.h"

#include "lib/io/pdf/include/PdfReader.h"

namespace io::pdf
{
    TEST(PdfReader, readPdf)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto reader = PdfReader(loggerFactory, 300);
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.pdf").getImage();
        auto const x = 2480;
        auto const y = 3508;
        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(4, real.channels());
        EXPECT_EQ(cv::Vec4b(0, 0, 0, 255), real.at<cv::Vec4b>(100, x / 2));         // black
        EXPECT_EQ(cv::Vec4b(0, 0, 255, 255), real.at<cv::Vec4b>(y / 2, 100));       // red
        EXPECT_EQ(cv::Vec4b(255, 255, 255, 255), real.at<cv::Vec4b>(y / 2, x / 2)); // white
        EXPECT_EQ(cv::Vec4b(255, 0, 0, 255), real.at<cv::Vec4b>(y / 2, x - 100));   // blue
        EXPECT_EQ(cv::Vec4b(0, 255, 0, 255), real.at<cv::Vec4b>(y - 100, x / 2));   // green
    }
}
