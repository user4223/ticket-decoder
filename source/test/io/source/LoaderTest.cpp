
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include "test/support/include/Loader.h"

#include "lib/utility/include/Logging.h"

#include <future>

namespace io::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    auto const ioEtc = []()
    { return support::Loader::getExecutableFolderPath() / "etc" / "io"; };

    TEST(Loader, syncDirectoryResult)
    {
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{})).load(ioEtc());
        EXPECT_TRUE(elements.hasCompleted());
        EXPECT_FALSE(elements.inProgress());
        EXPECT_EQ(5, elements.size());
    }

    TEST(Loader, syncDirectoryHandler)
    {
        auto count = 0;
        EXPECT_EQ(5, Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{}))
                         .load(ioEtc(), [&](auto &&inputElement)
                               {
                                    EXPECT_TRUE(inputElement.isValid());
                                    count++; }));
        EXPECT_EQ(5, count);
    }

    TEST(Loader, asyncDirectory)
    {
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{})).loadAsync(ioEtc());
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

    TEST(Loader, asyncDirectoryWithHandler)
    {
        auto result = std::vector<InputElement>{};
        auto future = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{}))
                          .loadAsync(ioEtc(), [&result](auto &&element)
                                     { result.emplace_back(std::move(element)); });
        EXPECT_EQ(5, future.get());
        EXPECT_EQ(5, result.size());
        for (int i = 0; i < result.size(); ++i)
        {
            EXPECT_TRUE(result[i].isValid());
            EXPECT_TRUE(result[i].getImage().cols > 0);
        }
    }

    TEST(Loader, imageFileResult)
    {
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{})).load(ioEtc() / "minimal.jpg");
        EXPECT_EQ(1, elements.size());
        EXPECT_EQ("minimal.jpg", std::filesystem::path(elements.get(0).getAnnotation()).filename().string());
    }

    TEST(Loader, imageFileHandler)
    {
        auto count = 0;
        EXPECT_EQ(1, Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{}))
                         .load(ioEtc() / "minimal.jpg", [&](auto &&inputElement)
                               {
                                    EXPECT_TRUE(inputElement.isValid());
                                    EXPECT_EQ("minimal.jpg", std::filesystem::path(inputElement.getAnnotation()).filename().string());
                                    count++; }));
        EXPECT_EQ(1, count);
    }

    TEST(Loader, pdfFile)
    {
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{})).load(ioEtc() / "minimal.pdf");
        EXPECT_EQ(1, elements.size());
        EXPECT_EQ("minimal.pdf", std::filesystem::path(elements.get(0).getAnnotation()).filename().string());
    }

    TEST(Loader, notExistingFile)
    {
        auto loader = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{}));
        EXPECT_THROW(loader.load(ioEtc() / "crappy.jpg"), std::runtime_error);
    }

    TEST(Loader, notExistingDirectory)
    {
        auto loader = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{}));
        EXPECT_THROW(loader.load(ioEtc() / "crappy" / "path"), std::runtime_error);
    }

    TEST(Loader, multipagePdfFile)
    {
        auto elements = Loader(loggerFactory, Reader::create(loggerFactory, api::ReaderOptions{})).load(ioEtc() / "two-page.pdf");
        EXPECT_EQ(2, elements.size());

        EXPECT_EQ("two-page.pdf[0]", std::filesystem::path(elements.get(0).getAnnotation()).filename().string());
        EXPECT_EQ(0, elements.get(0).getImage().at<std::uint8_t>(100, 100)); // 1st page -> black

        EXPECT_EQ("two-page.pdf[1]", std::filesystem::path(elements.get(1).getAnnotation()).filename().string());
        EXPECT_EQ(150, elements.get(1).getImage().at<std::uint8_t>(100, 100)); // 2nd page -> green
    }

    TEST(LoadResult, empty)
    {
        auto result = LoadResult(std::vector<InputElement>{});
        EXPECT_TRUE(result.hasCompleted());
        EXPECT_FALSE(result.inProgress());
        EXPECT_FALSE(result.waitForElementOrCompletion());
        EXPECT_EQ(0, result.size());
        EXPECT_THROW(result.get(0), std::runtime_error);
    }

    TEST(LoadResult, syncResult)
    {
        auto result = LoadResult({InputElement::fromFile("first", cv::Mat{})});
        EXPECT_TRUE(result.hasCompleted());
        EXPECT_FALSE(result.inProgress());
        EXPECT_TRUE(result.waitForElementOrCompletion());
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
        auto result = LoadResult([&](auto adder)
                                 {
                                    adder(future1.get());
                                    adder(future2.get()); });

        EXPECT_FALSE(result.hasCompleted());
        EXPECT_TRUE(result.inProgress());
        // EXPECT_FALSE(result.waitForElementOrCompletion());
        EXPECT_EQ(0, result.size());
        EXPECT_THROW(result.get(1), std::runtime_error);

        promise1.set_value({InputElement::fromFile("first", cv::Mat{})});
        EXPECT_FALSE(result.hasCompleted());
        EXPECT_TRUE(result.inProgress());
        EXPECT_TRUE(result.waitForElementOrCompletion());
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
        EXPECT_TRUE(result.waitForElementOrCompletion());
        EXPECT_EQ(2, result.size());
        EXPECT_EQ("first", result.get(0).getAnnotation());
        EXPECT_EQ("second", result.get(1).getAnnotation());
        EXPECT_THROW(result.get(2), std::runtime_error);
    }
}
