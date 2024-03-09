#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"
#include "lib/io/api/include/InputElement.h"

namespace io::api
{
    TEST(Writer, writePaths)
    {
        auto out = std::filesystem::temp_directory_path() / "out";
        auto writer = SinkManager::create()
                          .useDestination(out)
                          .build()
                          .get(InputElement::fromFile("ticket-decoder-test/document.pdf", cv::Mat{}));
        auto data = std::vector<std::uint8_t>{1, 2};
        EXPECT_EQ(out / "ticket-decoder-test/document.pdf.png", writer.write(cv::Mat{1, 1, CV_8UC1, data.data()}));
        EXPECT_EQ(out / "ticket-decoder-test/document.pdf.json", writer.write(std::string{}));
        EXPECT_EQ(out / "ticket-decoder-test/document.pdf.raw", writer.write(std::vector<uint8_t>{}));
    }

    TEST(Writer, writeMultiImagePaths)
    {
        auto out = std::filesystem::temp_directory_path() / "out";
        auto writer = SinkManager::create()
                          .useDestination(out)
                          .build()
                          .get(InputElement::fromFile("ticket-decoder-test/document.pdf", 3, cv::Mat{}));
        auto data = std::vector<std::uint8_t>{1, 2};
        EXPECT_EQ(out / "ticket-decoder-test/document.pdf_3.png", writer.write(cv::Mat{1, 1, CV_8UC1, data.data()}));
        EXPECT_EQ(out / "ticket-decoder-test/document.pdf_3.json", writer.write(std::string{}));
        EXPECT_EQ(out / "ticket-decoder-test/document.pdf_3.raw", writer.write(std::vector<uint8_t>{}));
    }
}
