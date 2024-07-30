#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/io/api/include/SinkManager.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    TEST(SinkManager, sourceAndDestinationDirectory1)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto manager = SinkManager::create(loggerFactory)
                           .useSource("input/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/input/folder/image.png", manager.deriveSinkPath("input/folder/image.png"));
    }

    TEST(SinkManager, sourceAndDestinationDirectory2)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input/folder");
        auto manager = SinkManager::create(loggerFactory)
                           .useSource("input/folder/")
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/input/folder/image.png", manager.deriveSinkPath("input/folder/image.png"));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .build();
        EXPECT_EQ("out/folder/image.png", manager.deriveSinkPath("folder/image.png"));
    }

    TEST(SinkManager, cameraSource)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("images");
        auto manager = SinkManager::create(loggerFactory)
                           .useDestination("out/")
                           .useSource("images/")
                           .build();
        EXPECT_EQ("out/Camera", manager.deriveSinkPath("Camera"));
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
}
