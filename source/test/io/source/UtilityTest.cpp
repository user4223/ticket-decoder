
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test/support/include/TestSupport.h"
#include "test/support/include/TempPath.h"

#include "lib/input/common/include/Utility.h"

#include <fstream>

namespace input::detail
{
    auto base = []()
    { return ::test::support::get().getIOPath(); };

    TEST(Utility, areFiles)
    {
        EXPECT_TRUE(areFiles({base() / "minimal.png"}));
        EXPECT_TRUE(areFiles({base() / "minimal.png", base() / "minimal.jpg"}));
    }

    TEST(Utility, areNoFiles)
    {
        EXPECT_FALSE(areFiles({}));
        EXPECT_FALSE(areFiles({base()}));
        EXPECT_FALSE(areFiles({base(), base().parent_path()}));
        EXPECT_FALSE(areFiles({base(), base() / "minimal.png"}));
        EXPECT_FALSE(areFiles({base() / "minimal.png", base()}));
        EXPECT_FALSE(areFiles({base() / "not_existing_79237459723495345jljlwjerlwjejrl.pdf"}));
    }

    TEST(Utility, areDirectories)
    {
        EXPECT_TRUE(areDirectories({std::filesystem::current_path()}));
        EXPECT_TRUE(areDirectories({base()}));
        EXPECT_TRUE(areDirectories({base(), base().parent_path()}));
    }

    TEST(Utility, areNoDirectories)
    {
        EXPECT_FALSE(areDirectories({}));
        EXPECT_FALSE(areDirectories({base() / "minimal.png"}));
        EXPECT_FALSE(areDirectories({base() / "minimal.png", base() / "minimal.jpg"}));
        EXPECT_FALSE(areDirectories({base(), base() / "minimal.png"}));
        EXPECT_FALSE(areDirectories({base() / "minimal.png", base()}));
        EXPECT_FALSE(areDirectories({base() / "not_existing_282345793479274359ljlsdjflsdf"}));
    }

    TEST(Utility, normalizeExtension)
    {
        EXPECT_EQ(".pdf", normalizeExtension(std::filesystem::path("bla") / "foo.pdf"));
        EXPECT_EQ(".pdf", normalizeExtension(std::filesystem::path("bla") / "foo.PDF"));
        EXPECT_EQ(".pdf", normalizeExtension(std::filesystem::path("bla") / "foo.pDf"));
        EXPECT_EQ(".blubber", normalizeExtension(std::filesystem::path("bla") / "foo.bluBBer"));
    }

    TEST(Utility, normalizeInvalidExtension)
    {
        EXPECT_EQ("", normalizeExtension(std::filesystem::path("bla") / ".foo"));
        EXPECT_EQ("", normalizeExtension(std::filesystem::path("bla") / "."));
        EXPECT_EQ("", normalizeExtension(std::filesystem::path("bla/")));
    }

    TEST(Utility, isFilePath)
    {
        EXPECT_TRUE(isFilePath("bla.txt"));
        EXPECT_TRUE(isFilePath("foo/bla.txt"));
        EXPECT_TRUE(isFilePath("bla.A"));
    }

    TEST(Utility, isNoFilePath)
    {
        EXPECT_FALSE(isFilePath("bla"));
        EXPECT_FALSE(isFilePath("foo/bla/"));
        EXPECT_FALSE(isFilePath("bla."));
        EXPECT_FALSE(isFilePath("bla/."));
        EXPECT_FALSE(isFilePath("bla/.."));
        EXPECT_FALSE(isFilePath("."));
        EXPECT_FALSE(isFilePath(".."));
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        ASSERT_THROW(ensureCompatiblePaths("input", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryExistsOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        std::filesystem::create_directories("input");
        ASSERT_NO_THROW(ensureCompatiblePaths("input", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryExistsOutputDirectoryExists)
    {
        auto tempPath = test::support::TempPath();
        std::filesystem::create_directories("input");
        std::filesystem::create_directories("output");
        ASSERT_NO_THROW(ensureCompatiblePaths("input", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        ASSERT_THROW(ensureCompatiblePaths("input.pdf", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        ASSERT_NO_THROW(ensureCompatiblePaths("input.pdf", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputDirectoryExists)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        std::filesystem::create_directories("output");
        ASSERT_NO_THROW(ensureCompatiblePaths("input.pdf", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputFile)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        ASSERT_NO_THROW(ensureCompatiblePaths("input.pdf", "output/output.pdf"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputFileExists)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        std::filesystem::create_directories("output");
        std::ofstream("output/output.pdf").close();
        ASSERT_NO_THROW(ensureCompatiblePaths("input.pdf", "output/output.pdf"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputNoFileOrDirectory)
    {
        ASSERT_THROW(ensureCompatiblePaths("/dev/null", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputNotExisting)
    {
        auto tempPath = test::support::TempPath();
        ASSERT_THROW(ensureCompatiblePaths("not_existing", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryExistsOutputFile)
    {
        auto tempPath = test::support::TempPath();
        std::filesystem::create_directories("input");
        ASSERT_THROW(ensureCompatiblePaths("input", "output.pdf"), std::invalid_argument);
    }
}
