#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"
#include "lib/io/api/include/InputElement.h"

#include "lib/utility/include/Logging.h"

#include "test/support/include/TestSupport.h"

namespace io::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);
    static auto const dummyImage = ::test::support::getDummyImage();

    TEST(SinkManager, relativeSourceAndRelativeDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        auto writer = manager.get(InputElement::fromFile("input/folder/image.png", dummyImage.clone()));
        EXPECT_EQ("out/input/folder/image.png_out.png", writer.write(dummyImage));
        EXPECT_EQ("out/input/folder/image.png_out.raw", writer.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/input/folder/image.png_out.json", writer.write("{}"));
    }

    TEST(SinkManager, absoluteSourceAndAbsoluteDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination(cwd / "out/")
                           .build();
        auto writer = manager.get(InputElement::fromFile(cwd / "input/folder/image.png", dummyImage.clone()));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.png", writer.write(dummyImage));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.raw", writer.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.json", writer.write("{}"));
    }

    TEST(SinkManager, relativeSourceAndAbsoluteDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination(cwd / "out/")
                           .build();
        auto writer = manager.get(InputElement::fromFile("input/folder/image.png", dummyImage.clone()));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.png", writer.write(dummyImage));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.raw", writer.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.json", writer.write("{}"));
    }

    TEST(SinkManager, absoluteSourceAndRelativeDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        auto writer = manager.get(InputElement::fromFile(cwd / "input/folder/image.png", dummyImage.clone()));
        EXPECT_EQ("out/input/folder/image.png_out.png", writer.write(dummyImage));
        EXPECT_EQ("out/input/folder/image.png_out.raw", writer.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/input/folder/image.png_out.json", writer.write("{}"));
    }

    TEST(SinkManager, nestedSourceAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input/folder");
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/input/folder/image.png", manager.deriveOutputElementPath(InputElement::fromFile("input/folder/image.png", ::test::support::getDummyImage())));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image.png", manager.deriveOutputElementPath(InputElement::fromFile("folder/image.png", ::test::support::getDummyImage())));
    }

    TEST(SinkManager, relativeSourceFileAndRelativeDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/blubber/image.png")
                           .build();
        EXPECT_EQ("out/blubber/image.png", manager.get(InputElement::fromFile("folder/other.png", dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, absoluteSourceFileAndAbsoluteDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("folder");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination(cwd / "out/blubber/image.png")
                           .build();
        EXPECT_EQ(cwd / "out/blubber/image.png", manager.get(InputElement::fromFile(cwd / "folder/other.png", dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, relativeSourceFileAndAbsoluteDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("folder");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination(cwd / "out/blubber/image.png")
                           .build();
        EXPECT_EQ(cwd / "out/blubber/image.png", manager.get(InputElement::fromFile("folder/other.png", dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, absoluteSourceFileAndRelativeDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("folder");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/blubber/image.png")
                           .build();
        EXPECT_EQ("out/blubber/image.png", manager.get(InputElement::fromFile(cwd / "folder/other.png", dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, sourceFileMultipleIndexesAndDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/blubber/image.png")
                           .build();
        EXPECT_EQ("out/blubber/image.png", manager.get(InputElement::fromFile("folder/other.png", 0, dummyImage.clone())).write(dummyImage));
        EXPECT_EQ("out/blubber/image_1.png", manager.get(InputElement::fromFile("folder/other.png", 1, dummyImage.clone())).write(dummyImage));
        EXPECT_EQ("out/blubber/image_15.png", manager.get(InputElement::fromFile("folder/other.png", 15, dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, sourceFileMultipleDestinationFileTypes)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/blubber/image.png")
                           .build();
        auto writer0 = manager.get(InputElement::fromFile("folder/other.png", 0, dummyImage.clone()));
        EXPECT_EQ("out/blubber/image.png", writer0.write(dummyImage));
        EXPECT_EQ("out/blubber/image.png.raw", writer0.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/blubber/image.png.json", writer0.write("{}"));
        auto writer5 = manager.get(InputElement::fromFile("folder/other.png", 5, dummyImage.clone()));
        EXPECT_EQ("out/blubber/image_5.png", writer5.write(dummyImage));
        EXPECT_EQ("out/blubber/image_5.png.raw", writer5.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/blubber/image_5.png.json", writer5.write("{}"));
    }

    TEST(SinkManager, cameraSource)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("images");
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        auto writer = manager.get(InputElement::fromCamera(dummyImage.clone()));
        EXPECT_EQ("out/camera_out.png", writer.write(dummyImage));
        EXPECT_EQ("out/camera_out.raw", writer.write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/camera_out.json", writer.write("{}"));
    }

    TEST(SinkManager, isFilePath)
    {
        EXPECT_TRUE(SinkManager::isFilePath("bla.txt"));
        EXPECT_TRUE(SinkManager::isFilePath("foo/bla.txt"));
        EXPECT_TRUE(SinkManager::isFilePath("bla.A"));
    }

    TEST(SinkManager, isNoFilePath)
    {
        EXPECT_FALSE(SinkManager::isFilePath("bla"));
        EXPECT_FALSE(SinkManager::isFilePath("foo/bla/"));
        EXPECT_FALSE(SinkManager::isFilePath("bla."));
        EXPECT_FALSE(SinkManager::isFilePath("bla/."));
        EXPECT_FALSE(SinkManager::isFilePath("bla/.."));
        EXPECT_FALSE(SinkManager::isFilePath("."));
        EXPECT_FALSE(SinkManager::isFilePath(".."));
    }
}
