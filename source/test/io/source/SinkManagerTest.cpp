#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"

namespace io::api
{
    TEST(SinkManager, sourceAndDestinationDirectory1)
    {
        auto manager = SinkManager::create()
                           .useSource("input/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/folder/image.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceAndDestinationDirectory2)
    {
        auto manager = SinkManager::create()
                           .useSource("input/folder/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/image", manager.deriveSinkPath("input/folder/image.png"));
        EXPECT_EQ("out/image.json", manager.deriveSinkPath("input/folder/image.png", ".json"));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        auto manager = SinkManager::create()
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image", manager.deriveSinkPath("folder/image.png"));
        EXPECT_EQ("out/folder/image.raw", manager.deriveSinkPath("folder/image.png", ".raw"));
    }

    TEST(Sink, writePaths)
    {
        auto out = std::filesystem::temp_directory_path() / "out";
        auto sink = SinkManager::create()
                        .useDestination(out)
                        .build()
                        .get(InputElement::fromFile("ticket-decoder-test/document.pdf", cv::Mat{}));
        auto data = std::vector<std::uint8_t>{1, 2};
        EXPECT_EQ(out / "ticket-decoder-test/document.png", sink.write(cv::Mat{1, 1, CV_8UC1, data.data()}));
        EXPECT_EQ(out / "ticket-decoder-test/document.json", sink.write(std::string{}));
        EXPECT_EQ(out / "ticket-decoder-test/document.raw", sink.write(std::vector<uint8_t>{}));
    }

    TEST(Sink, writeMultiImagePaths)
    {
        auto out = std::filesystem::temp_directory_path() / "out";
        auto sink = SinkManager::create()
                        .useDestination(out)
                        .build()
                        .get(InputElement::fromFile("ticket-decoder-test/document.pdf", 3, cv::Mat{}));
        auto data = std::vector<std::uint8_t>{1, 2};
        EXPECT_EQ(out / "ticket-decoder-test/document_3.png", sink.write(cv::Mat{1, 1, CV_8UC1, data.data()}));
        EXPECT_EQ(out / "ticket-decoder-test/document_3.json", sink.write(std::string{}));
        EXPECT_EQ(out / "ticket-decoder-test/document_3.raw", sink.write(std::vector<uint8_t>{}));
    }
}
