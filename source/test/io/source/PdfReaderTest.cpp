
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

    TEST(PdfReader, singlePageNoSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0}), PdfReader::selectedPages(api::ReadOptions{{}}, 1));
    }

    TEST(PdfReader, multiplePagesNoSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0, 1, 2, 3, 4}), PdfReader::selectedPages(api::ReadOptions{{}}, 5));
    }

    TEST(PdfReader, noPagesNoSelection)
    {
        EXPECT_TRUE(PdfReader::selectedPages(api::ReadOptions{{}}, 0).empty());
    }

    TEST(PdfReader, singlePageSingleSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0}), PdfReader::selectedPages(api::ReadOptions{{0}}, 1));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages(api::ReadOptions{{1}}, 1));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages(api::ReadOptions{{(unsigned int)-1}}, 1));
    }

    TEST(PdfReader, multiplePagesSingleSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({0}), PdfReader::selectedPages(api::ReadOptions{{0}}, 5));
        EXPECT_EQ(std::vector<unsigned int>({2}), PdfReader::selectedPages(api::ReadOptions{{2}}, 5));
        EXPECT_EQ(std::vector<unsigned int>({4}), PdfReader::selectedPages(api::ReadOptions{{4}}, 5));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages(api::ReadOptions{{5}}, 5));
    }

    TEST(PdfReader, noPagesSingleSelection)
    {
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages(api::ReadOptions{{0}}, 0));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages(api::ReadOptions{{1}}, 0));
        EXPECT_EQ(std::vector<unsigned int>({}), PdfReader::selectedPages(api::ReadOptions{{23}}, 0));
    }

    TEST(PdfReader, multiplePagesMultipleSelections)
    {
        EXPECT_EQ(std::vector<unsigned int>({0, 2, 4}), PdfReader::selectedPages(api::ReadOptions{{0, 2, 4}}, 5));
        EXPECT_EQ(std::vector<unsigned int>({0, 1}), PdfReader::selectedPages(api::ReadOptions{{0, 1}}, 2));
        EXPECT_EQ(std::vector<unsigned int>({1, 3}), PdfReader::selectedPages(api::ReadOptions{{1, 3, 5}}, 5));
        EXPECT_EQ(std::vector<unsigned int>({0, 1, 2, 3, 4}), PdfReader::selectedPages(api::ReadOptions{{0, 1, 2, 3, 4}}, 5));
    }
}
