#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/output/api/include/SinkManager.h"
#include "lib/input/api/include/InputElement.h"

#include "test/support/include/TestSupport.h"

namespace io::api
{
    cv::Mat getDummyImage()
    {
        return ::test::support::get().getDummyImage();
    }

    TEST(SinkManager, relativeSourceAndRelativeDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/")
                           .build();
        auto writer = manager.get(input::api::InputElement::fromFile("input/folder/image.png", getDummyImage()));
        EXPECT_EQ("out/input/folder/image.png_out.png", writer->write(getDummyImage()));
        EXPECT_EQ("out/input/folder/image.png_out.raw", writer->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/input/folder/image.png_out.json", writer->write("{}"));
    }

    TEST(SinkManager, absoluteSourceAndAbsoluteDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath(cwd / "out/")
                           .build();
        auto writer = manager.get(input::api::InputElement::fromFile(cwd / "input/folder/image.png", getDummyImage()));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.png", writer->write(getDummyImage()));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.raw", writer->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.json", writer->write("{}"));
    }

    TEST(SinkManager, relativeSourceAndAbsoluteDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath(cwd / "out/")
                           .build();
        auto writer = manager.get(input::api::InputElement::fromFile("input/folder/image.png", getDummyImage()));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.png", writer->write(getDummyImage()));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.raw", writer->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ(cwd / "out/input/folder/image.png_out.json", writer->write("{}"));
    }

    TEST(SinkManager, absoluteSourceAndRelativeDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/")
                           .build();
        auto writer = manager.get(input::api::InputElement::fromFile(cwd / "input/folder/image.png", getDummyImage()));
        EXPECT_EQ("out/input/folder/image.png_out.png", writer->write(getDummyImage()));
        EXPECT_EQ("out/input/folder/image.png_out.raw", writer->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/input/folder/image.png_out.json", writer->write("{}"));
    }

    TEST(SinkManager, nestedSourceAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("input/folder");
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/")
                           .build();
        EXPECT_EQ("out/input/folder/image.png_out.png", manager.get(input::api::InputElement::fromFile("input/folder/image.png", getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, sourceFileAndDestinationDirectory)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/")
                           .build();
        EXPECT_EQ("out/folder/image.png_out.png", manager.get(input::api::InputElement::fromFile("folder/image.png", getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, relativeSourceFileAndRelativeDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/blubber/image.png")
                           .build();
        EXPECT_EQ("out/blubber/image.png", manager.get(input::api::InputElement::fromFile("folder/other.png", getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, absoluteSourceFileAndAbsoluteDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("folder");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath(cwd / "out/blubber/image.png")
                           .build();
        EXPECT_EQ(cwd / "out/blubber/image.png", manager.get(input::api::InputElement::fromFile(cwd / "folder/other.png", getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, relativeSourceFileAndAbsoluteDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("folder");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath(cwd / "out/blubber/image.png")
                           .build();
        EXPECT_EQ(cwd / "out/blubber/image.png", manager.get(input::api::InputElement::fromFile("folder/other.png", getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, absoluteSourceFileAndRelativeDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("folder");
        auto const cwd = std::filesystem::current_path();
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/blubber/image.png")
                           .build();
        EXPECT_EQ("out/blubber/image.png", manager.get(input::api::InputElement::fromFile(cwd / "folder/other.png", getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, sourceFileMultipleIndexesAndDestinationFile)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/blubber/image.png")
                           .build();
        EXPECT_EQ("out/blubber/image.png", manager.get(input::api::InputElement::fromFile("folder/other.png", 0, getDummyImage()))->write(getDummyImage()));
        EXPECT_EQ("out/blubber/image_1.png", manager.get(input::api::InputElement::fromFile("folder/other.png", 1, getDummyImage()))->write(getDummyImage()));
        EXPECT_EQ("out/blubber/image_15.png", manager.get(input::api::InputElement::fromFile("folder/other.png", 15, getDummyImage()))->write(getDummyImage()));
    }

    TEST(SinkManager, sourceFileMultipleDestinationFileTypes)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/blubber/image.png")
                           .build();
        auto writer0 = manager.get(input::api::InputElement::fromFile("folder/other.png", 0, getDummyImage()));
        EXPECT_EQ("out/blubber/image.png", writer0->write(getDummyImage()));
        EXPECT_EQ("out/blubber/image.png.raw", writer0->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/blubber/image.png.json", writer0->write("{}"));
        auto writer5 = manager.get(input::api::InputElement::fromFile("folder/other.png", 5, getDummyImage()));
        EXPECT_EQ("out/blubber/image_5.png", writer5->write(getDummyImage()));
        EXPECT_EQ("out/blubber/image_5.png.raw", writer5->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/blubber/image_5.png.json", writer5->write("{}"));
    }

    TEST(SinkManager, sourceDirectoryWithoutInputElementPathThrows)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/blubber/")
                           .build();
        EXPECT_THROW(manager.get(), std::runtime_error);
    }

    TEST(SinkManager, sourceFileWithoutInputElementPathDoesNotThrow)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/blubber/")
                           .build();
        auto writer = manager.get("input/file.pdf");
        EXPECT_EQ("out/blubber/input/file.pdf_out.json", writer->write("{}"));
    }

    TEST(SinkManager, cameraSource)
    {
        std::filesystem::current_path(std::filesystem::temp_directory_path());
        std::filesystem::create_directories("images");
        auto manager = SinkManager::create(::test::support::get().getContext())
                           .useDestinationPath("out/")
                           .build();
        auto writer = manager.get(input::api::InputElement::fromCamera(getDummyImage()));
        EXPECT_EQ("out/camera_out.png", writer->write(getDummyImage()));
        EXPECT_EQ("out/camera_out.raw", writer->write(std::vector<std::uint8_t>{23u, 42u}));
        EXPECT_EQ("out/camera_out.json", writer->write("{}"));
    }

    TEST(SinkManager, noDestination)
    {
        EXPECT_THROW(SinkManager::create(::test::support::get().getContext())
                         .build(),
                     std::runtime_error);
    }

    TEST(SinkManager, moreThanOneDestination)
    {
        EXPECT_THROW(SinkManager::create(::test::support::get().getContext())
                         .useDestinationPath("path/")
                         .useDestinationStream(std::cout)
                         .build(),
                     std::runtime_error);
    }
}
