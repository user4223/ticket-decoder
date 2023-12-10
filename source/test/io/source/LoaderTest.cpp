
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include "test/support/include/Loader.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    auto const ioEtc = []()
    { return support::Loader::getExecutableFolderPath() / "etc" / "io"; };

    TEST(Loader, directory)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc());
        EXPECT_EQ(5, elements.size());
    }

    TEST(Loader, imageFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "minimal.jpg");
        EXPECT_EQ(1, elements.size());
    }

    TEST(Loader, pdfFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "minimal.pdf");
        EXPECT_EQ(1, elements.size());
    }

    TEST(Loader, notExisting)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "crappy.jpg");
        EXPECT_EQ(0, elements.size());
    }

    TEST(Loader, multipagePdfFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "two-page.pdf");
        EXPECT_EQ(2, elements.size());

        EXPECT_EQ("two-page.pdf[0]", std::filesystem::path(elements[0].annotation).filename().string());
        EXPECT_EQ(cv::Vec4b(0, 0, 0, 255), elements[0].image.at<cv::Vec4b>(100, 100)); // 1st page -> black

        EXPECT_EQ("two-page.pdf[1]", std::filesystem::path(elements[1].annotation).filename().string());
        EXPECT_EQ(cv::Vec4b(0, 255, 0, 255), elements[1].image.at<cv::Vec4b>(100, 100)); // 2nd page -> green
    }
}
