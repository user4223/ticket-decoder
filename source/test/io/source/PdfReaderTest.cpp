
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "test/support/include/Loader.h"

#include "lib/io/pdf/include/PdfReader.h"

namespace io::pdf
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    static auto const x = 2480; // 300 dpi -> change when you change dpi values
    static auto const y = 3508;

    TEST(PdfReader, readColoredPdf)
    {
        auto reader = PdfReader(loggerFactory, api::ReaderOptions{300, {}, false});
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.pdf").getImage();

        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(4, real.channels());
        EXPECT_EQ(cv::Vec4b(0, 0, 0, 255), real.at<cv::Vec4b>(100, x / 2));         // black
        EXPECT_EQ(cv::Vec4b(0, 0, 255, 255), real.at<cv::Vec4b>(y / 2, 100));       // red
        EXPECT_EQ(cv::Vec4b(255, 255, 255, 255), real.at<cv::Vec4b>(y / 2, x / 2)); // white
        EXPECT_EQ(cv::Vec4b(255, 0, 0, 255), real.at<cv::Vec4b>(y / 2, x - 100));   // blue
        EXPECT_EQ(cv::Vec4b(0, 255, 0, 255), real.at<cv::Vec4b>(y - 100, x / 2));   // green
    }

    TEST(PdfReader, readGrayPdf)
    {
        auto reader = PdfReader(loggerFactory, {});
        auto const real = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "minimal.pdf").getImage();

        EXPECT_EQ(x, real.size().width);
        EXPECT_EQ(y, real.size().height);
        EXPECT_EQ(1, real.channels());
        EXPECT_EQ(0, real.at<std::uint8_t>(100, x / 2));       // black
        EXPECT_EQ(77, real.at<std::uint8_t>(y / 2, 100));      // red
        EXPECT_EQ(255, real.at<std::uint8_t>(y / 2, x / 2));   // white
        EXPECT_EQ(28, real.at<std::uint8_t>(y / 2, x - 100));  // blue
        EXPECT_EQ(150, real.at<std::uint8_t>(y - 100, x / 2)); // green
    }

    TEST(PdfReader, readMultiPagePdf)
    {
        auto reader = PdfReader(loggerFactory, {300, {}, false});
        auto result = reader.read(support::Loader::getExecutableFolderPath() / "etc" / "io" / "two-page.pdf");
        EXPECT_TRUE(result.isMultiPart());
        EXPECT_EQ(2, result.getImages().size());

        auto const first = result.getImages()[0];
        EXPECT_EQ(cv::Vec4b(0, 0, 0, 255), first.at<cv::Vec4b>(100, x / 2));         // black
        EXPECT_EQ(cv::Vec4b(0, 0, 255, 255), first.at<cv::Vec4b>(y / 2, 100));       // red
        EXPECT_EQ(cv::Vec4b(255, 255, 255, 255), first.at<cv::Vec4b>(y / 2, x / 2)); // white
        EXPECT_EQ(cv::Vec4b(255, 0, 0, 255), first.at<cv::Vec4b>(y / 2, x - 100));   // blue
        EXPECT_EQ(cv::Vec4b(0, 255, 0, 255), first.at<cv::Vec4b>(y - 100, x / 2));   // green

        auto const second = result.getImages()[1];
        EXPECT_EQ(cv::Vec4b(0, 255, 0, 255), second.at<cv::Vec4b>(100, x / 2));       // green
        EXPECT_EQ(cv::Vec4b(0, 0, 255, 255), second.at<cv::Vec4b>(y / 2, 100));       // red
        EXPECT_EQ(cv::Vec4b(255, 255, 255, 255), second.at<cv::Vec4b>(y / 2, x / 2)); // white
        EXPECT_EQ(cv::Vec4b(255, 0, 0, 255), second.at<cv::Vec4b>(y / 2, x - 100));   // blue
        EXPECT_EQ(cv::Vec4b(0, 0, 0, 255), second.at<cv::Vec4b>(y - 100, x / 2));     // black
    }

    TEST(PdfReader, singlePageNoSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0}), PdfReader::selectedPages({}, 1));
    }

    TEST(PdfReader, multiplePagesNoSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0, 1, 2, 3, 4}), PdfReader::selectedPages({}, 5));
    }

    TEST(PdfReader, noPagesNoSelection)
    {
        EXPECT_TRUE(PdfReader::selectedPages({}, 0).empty());
    }

    TEST(PdfReader, singlePageSingleSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0}), PdfReader::selectedPages({0}, 1));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages({1}, 1));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages({(unsigned int)-1}, 1));
    }

    TEST(PdfReader, multiplePagesSingleSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0}), PdfReader::selectedPages({0}, 5));
        EXPECT_EQ(std::vector<unsigned int>({2}), PdfReader::selectedPages({2}, 5));
        EXPECT_EQ(std::vector<unsigned int>({4}), PdfReader::selectedPages({4}, 5));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages({5}, 5));
    }

    TEST(PdfReader, noPagesSingleSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages({0}, 0));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages({1}, 0));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages({23}, 0));
    }

    TEST(PdfReader, multiplePagesMultipleSelections)
    {
        EXPECT_EQ(std::vector<unsigned int>({0, 2, 4}), PdfReader::selectedPages({0, 2, 4}, 5));
        EXPECT_EQ(std::vector<unsigned int>({0, 1}), PdfReader::selectedPages({0, 1}, 2));
        EXPECT_EQ(std::vector<unsigned int>({1, 3}), PdfReader::selectedPages({1, 3, 5}, 5));
        EXPECT_EQ(std::vector<unsigned int>({0, 1, 2, 3, 4}), PdfReader::selectedPages({0, 1, 2, 3, 4}, 5));
    }
}
