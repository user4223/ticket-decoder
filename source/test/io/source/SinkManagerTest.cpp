#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"
#include "lib/io/api/include/InputElement.h"

#include "lib/utility/include/Logging.h"

#include "test/support/include/TestSupport.h"

namespace io::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    TEST(SinkManager, sourceAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto manager = SinkManager::create(loggerFactory)
                           .useSource("input/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/input/folder/image.png", manager.deriveOutputElementPath(InputElement::fromFile("input/folder/image.png", ::test::support::getDummyImage())));
    }

    TEST(SinkManager, nestedSourceAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input/folder");
        auto manager = SinkManager::create(loggerFactory)
                           .useSource("input/folder/")
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

    TEST(SinkManager, sourceFileAndDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/blubber/image.png")
                           .build();
        auto const dummyImage = ::test::support::getDummyImage();
        EXPECT_EQ("out/blubber/image.png", manager.get(InputElement::fromFile("folder/other.png", dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, sourceFileMultipleCodesAndDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/blubber/image.png")
                           .build();
        auto const dummyImage = ::test::support::getDummyImage();
        EXPECT_EQ("out/blubber/image.png", manager.get(InputElement::fromFile("folder/other.png", 0, dummyImage.clone())).write(dummyImage));
        EXPECT_EQ("out/blubber/image_1.png", manager.get(InputElement::fromFile("folder/other.png", 1, dummyImage.clone())).write(dummyImage));
        EXPECT_EQ("out/blubber/image_15.png", manager.get(InputElement::fromFile("folder/other.png", 15, dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, cameraSource)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("images");
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .useSource("images/")
                           .build();
        auto const dummyImage = ::test::support::getDummyImage();
        EXPECT_EQ("out/camera_out.png", manager.get(InputElement::fromCamera(dummyImage.clone())).write(dummyImage));
    }

    TEST(SinkManager, deriveOutputDirectoryPathFromRelativeDirectories)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("in");
        EXPECT_EQ("out/in", deriveOutputDirectoryPath("./in/", "./out/"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath("./in", "./out"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath("in", "./out"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath("in", "out"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath("in/some/..", "out"));
        std::filesystem::create_directories("in/crazy");
        EXPECT_EQ("out/in/crazy", deriveOutputDirectoryPath("in/crazy/", "out"));
        std::ofstream("in/file.txt").close();
        EXPECT_EQ("out/in", deriveOutputDirectoryPath("./in/file.txt", "./out"));
        std::ofstream("in/crazy/file.txt").close();
        EXPECT_EQ("out/in/crazy", deriveOutputDirectoryPath("./in/crazy/file.txt", "./out"));
    }

    TEST(SinkManager, deriveOutputDirectoryPathFromAbsoluteDirectories)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("in");
        auto const cwd = std::filesystem::current_path();
        EXPECT_EQ(cwd / "out/in", deriveOutputDirectoryPath(cwd / "in/", cwd / "out/"));
        EXPECT_EQ(cwd / "out/in", deriveOutputDirectoryPath(cwd / "in", cwd / "out"));
        std::ofstream("in/file.txt").close();
        EXPECT_EQ(cwd / "out/in", deriveOutputDirectoryPath(cwd / "in/file.txt", cwd / "out"));
    }

    TEST(SinkManager, deriveOutputDirectoryPathFromMixedDirectories)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("in");
        auto const cwd = std::filesystem::current_path();
        EXPECT_EQ(cwd / "out/in", deriveOutputDirectoryPath("in/", cwd / "out/"));
        EXPECT_EQ(cwd / "out/in", deriveOutputDirectoryPath("in", cwd / "out"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath(cwd / "in/", "out/"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath(cwd / "in", "out"));
        std::ofstream("in/file.txt").close();
        EXPECT_EQ(cwd / "out/in", deriveOutputDirectoryPath("in/file.txt", cwd / "out"));
        EXPECT_EQ("out/in", deriveOutputDirectoryPath(cwd / "in/file.txt", "out"));
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
