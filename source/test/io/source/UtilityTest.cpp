
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test/support/include/TestSupport.h"

#include "lib/io/api/include/Utility.h"

#include <fstream>

namespace io::api
{
    auto base = []()
    { return ::test::support::getExecutableFolderPath() / "etc" / "io"; };

    TEST(Utility, areFiles)
    {
        EXPECT_TRUE(utility::areFiles({base() / "minimal.png"}));
        EXPECT_TRUE(utility::areFiles({base() / "minimal.png", base() / "minimal.jpg"}));
    }

    TEST(Utility, areNoFiles)
    {
        EXPECT_FALSE(utility::areFiles({}));
        EXPECT_FALSE(utility::areFiles({base()}));
        EXPECT_FALSE(utility::areFiles({base(), base().parent_path()}));
        EXPECT_FALSE(utility::areFiles({base(), base() / "minimal.png"}));
        EXPECT_FALSE(utility::areFiles({base() / "minimal.png", base()}));
        EXPECT_FALSE(utility::areFiles({base() / "not_existing_79237459723495345jljlwjerlwjejrl.pdf"}));
    }

    TEST(Utility, areDirectories)
    {
        EXPECT_TRUE(utility::areDirectories({std::filesystem::current_path()}));
        EXPECT_TRUE(utility::areDirectories({base()}));
        EXPECT_TRUE(utility::areDirectories({base(), base().parent_path()}));
    }

    TEST(Utility, areNoDirectories)
    {
        EXPECT_FALSE(utility::areDirectories({}));
        EXPECT_FALSE(utility::areDirectories({base() / "minimal.png"}));
        EXPECT_FALSE(utility::areDirectories({base() / "minimal.png", base() / "minimal.jpg"}));
        EXPECT_FALSE(utility::areDirectories({base(), base() / "minimal.png"}));
        EXPECT_FALSE(utility::areDirectories({base() / "minimal.png", base()}));
        EXPECT_FALSE(utility::areDirectories({base() / "not_existing_282345793479274359ljlsdjflsdf"}));
    }

    TEST(Utility, normalizeExtension)
    {
        EXPECT_EQ(".pdf", utility::normalizeExtension(std::filesystem::path("bla") / "foo.pdf"));
        EXPECT_EQ(".pdf", utility::normalizeExtension(std::filesystem::path("bla") / "foo.PDF"));
        EXPECT_EQ(".pdf", utility::normalizeExtension(std::filesystem::path("bla") / "foo.pDf"));
        EXPECT_EQ(".blubber", utility::normalizeExtension(std::filesystem::path("bla") / "foo.bluBBer"));
    }

    TEST(Utility, normalizeInvalidExtension)
    {
        EXPECT_EQ("", utility::normalizeExtension(std::filesystem::path("bla") / ".foo"));
        EXPECT_EQ("", utility::normalizeExtension(std::filesystem::path("bla") / "."));
        EXPECT_EQ("", utility::normalizeExtension(std::filesystem::path("bla/")));
    }

    TEST(Utility, isFilePath)
    {
        EXPECT_TRUE(utility::isFilePath("bla.txt"));
        EXPECT_TRUE(utility::isFilePath("foo/bla.txt"));
        EXPECT_TRUE(utility::isFilePath("bla.A"));
    }

    TEST(Utility, isNoFilePath)
    {
        EXPECT_FALSE(utility::isFilePath("bla"));
        EXPECT_FALSE(utility::isFilePath("foo/bla/"));
        EXPECT_FALSE(utility::isFilePath("bla."));
        EXPECT_FALSE(utility::isFilePath("bla/."));
        EXPECT_FALSE(utility::isFilePath("bla/.."));
        EXPECT_FALSE(utility::isFilePath("."));
        EXPECT_FALSE(utility::isFilePath(".."));
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        ASSERT_THROW(utility::ensureCompatiblePaths("input", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryExistsOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        std::filesystem::create_directories("input");
        ASSERT_NO_THROW(utility::ensureCompatiblePaths("input", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryExistsOutputDirectoryExists)
    {
        auto tempPath = test::support::TempPath();
        std::filesystem::create_directories("input");
        std::filesystem::create_directories("output");
        ASSERT_NO_THROW(utility::ensureCompatiblePaths("input", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        ASSERT_THROW(utility::ensureCompatiblePaths("input.pdf", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputDirectory)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        ASSERT_NO_THROW(utility::ensureCompatiblePaths("input.pdf", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputDirectoryExists)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        std::filesystem::create_directories("output");
        ASSERT_NO_THROW(utility::ensureCompatiblePaths("input.pdf", "output"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputFile)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        ASSERT_NO_THROW(utility::ensureCompatiblePaths("input.pdf", "output/output.pdf"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputFileExistsOutputFileExists)
    {
        auto tempPath = test::support::TempPath();
        std::ofstream("input.pdf").close();
        std::filesystem::create_directories("output");
        std::ofstream("output/output.pdf").close();
        ASSERT_NO_THROW(utility::ensureCompatiblePaths("input.pdf", "output/output.pdf"));
        EXPECT_TRUE(std::filesystem::exists("output"));
        EXPECT_TRUE(std::filesystem::is_directory("output"));
    }

    TEST(Utility, ensureCompatiblePaths_InputNoFileOrDirectory)
    {
        ASSERT_THROW(utility::ensureCompatiblePaths("/dev/null", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputNotExisting)
    {
        auto tempPath = test::support::TempPath();
        ASSERT_THROW(utility::ensureCompatiblePaths("not_existing", "output"), std::invalid_argument);
    }

    TEST(Utility, ensureCompatiblePaths_InputDirectoryExistsOutputFile)
    {
        auto tempPath = test::support::TempPath();
        std::filesystem::create_directories("input");
        ASSERT_THROW(utility::ensureCompatiblePaths("input", "output.pdf"), std::invalid_argument);
    }
}
