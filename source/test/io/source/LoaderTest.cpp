
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

    TEST(Loader, syncDirectory)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).load(ioEtc());
        EXPECT_TRUE(elements.hasCompleted());
        EXPECT_FALSE(elements.inProgress());
        EXPECT_EQ(5, elements.size());
    }

    TEST(Loader, asyncDirectory)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}})).loadAsync(ioEtc());
        while (elements.inProgress())
        {
            std::this_thread::yield();
        }
        EXPECT_TRUE(elements.hasCompleted());
        EXPECT_EQ(5, elements.size());
        for (int i = 0; i < elements.size(); ++i)
        {
            EXPECT_TRUE(elements.get(i).isValid());
            EXPECT_TRUE(elements.get(i).getImage().cols > 0);
        }
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

    TEST(Loader, notExistingFile)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto loader = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}}));
        EXPECT_THROW(loader.load(ioEtc() / "crappy.jpg"), std::runtime_error);
    }

    TEST(Loader, notExistingDirectory)
    {
        auto loggerFactory = ::utility::LoggerFactory::create();
        auto loader = Loader(loggerFactory, Reader::create(loggerFactory, api::ReadOptions{{}, {}}));
        EXPECT_THROW(loader.load(ioEtc() / "crappy" / "path"), std::runtime_error);
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
        auto result = LoadResult(std::vector<InputElement>{});
        EXPECT_TRUE(result.hasCompleted());
        EXPECT_FALSE(result.inProgress());
        EXPECT_EQ(0, result.size());
        EXPECT_THROW(result.get(0), std::runtime_error);
    }

    TEST(LoadResult, syncResult)
    {
        auto result = LoadResult({InputElement::fromFile("first", cv::Mat{})});
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

        promise1.set_value({InputElement::fromFile("first", cv::Mat{})});
        EXPECT_FALSE(result.hasCompleted());
        EXPECT_TRUE(result.inProgress());
        while (result.size() < 1)
        {
            std::this_thread::yield();
        }
        EXPECT_EQ("first", result.get(0).getAnnotation());

        promise2.set_value({InputElement::fromFile("second", cv::Mat{})});
        while (!result.hasCompleted())
        {
            std::this_thread::yield();
        }
        EXPECT_FALSE(result.inProgress());
        EXPECT_EQ(2, result.size());
        EXPECT_EQ("first", result.get(0).getAnnotation());
        EXPECT_EQ("second", result.get(1).getAnnotation());
        EXPECT_THROW(result.get(2), std::runtime_error);
    }
}
