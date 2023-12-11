
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include "test/support/include/Loader.h"

#include "lib/utility/include/Logging.h"

#include <future>

namespace io::api
{
    auto const ioEtc = []()
    { return support::Loader::getExecutableFolderPath() / "etc" / "io"; };

    TEST(Loader, directory)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc());
        EXPECT_EQ(5, elements.size());
    }

    TEST(Loader, imageFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "minimal.jpg");
        EXPECT_EQ(1, elements.size());
    }

    TEST(Loader, pdfFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "minimal.pdf");
        EXPECT_EQ(1, elements.size());
    }

    TEST(Loader, notExisting)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "crappy.jpg");
        EXPECT_EQ(0, elements.size());
    }

    TEST(Loader, multipagePdfFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc() / "two-page.pdf");
        EXPECT_EQ(2, elements.size());

        EXPECT_EQ("two-page.pdf[0]", std::filesystem::path(elements.get(0).getAnnotation()).filename().string());
        EXPECT_EQ(cv::Vec4b(0, 0, 0, 255), elements.get(0).getImage().at<cv::Vec4b>(100, 100)); // 1st page -> black

        EXPECT_EQ("two-page.pdf[1]", std::filesystem::path(elements.get(1).getAnnotation()).filename().string());
        EXPECT_EQ(cv::Vec4b(0, 255, 0, 255), elements.get(1).getImage().at<cv::Vec4b>(100, 100)); // 2nd page -> green
    }

    TEST(LoadResult, empty)
    {
        auto result = LoadResult();
        EXPECT_TRUE(result.hasCompleted());
        EXPECT_FALSE(result.inProgress());
        EXPECT_EQ(0, result.size());
        EXPECT_THROW(result.get(0), std::runtime_error);
    }

    TEST(LoadResult, syncResult)
    {
        auto result = LoadResult({InputElement{"first", cv::Mat{}}});
        EXPECT_TRUE(result.hasCompleted());
        EXPECT_FALSE(result.inProgress());
        EXPECT_EQ(1, result.size());
        EXPECT_EQ("first", result.get(0).getAnnotation());
        EXPECT_TRUE(result.get(0).getImage().empty());
        EXPECT_THROW(result.get(1), std::runtime_error);
    }

    TEST(LoadResult, asyncResult)
    {
        auto promise1 = std::promise<InputElement>();
        auto future1 = promise1.get_future();
        auto promise2 = std::promise<InputElement>();
        auto future2 = promise2.get_future();
        auto result = LoadResult([&](LoadResult &result)
                                 { 
                                    result.add(future1.get()); 
                                    result.add(future2.get()); });

        EXPECT_FALSE(result.hasCompleted());
        EXPECT_TRUE(result.inProgress());
        EXPECT_EQ(0, result.size());
        EXPECT_THROW(result.get(1), std::runtime_error);

        promise1.set_value({InputElement{"first", cv::Mat{}}});
        EXPECT_FALSE(result.hasCompleted());
        EXPECT_TRUE(result.inProgress());
        while (result.size() < 1)
        {
        }
        EXPECT_EQ("first", result.get(0).getAnnotation());

        promise2.set_value({InputElement{"second", cv::Mat{}}});
        while (!result.hasCompleted())
        {
        }
        EXPECT_FALSE(result.inProgress());
        EXPECT_EQ(2, result.size());
        EXPECT_EQ("first", result.get(0).getAnnotation());
        EXPECT_EQ("second", result.get(1).getAnnotation());
        EXPECT_THROW(result.get(2), std::runtime_error);
    }
}
