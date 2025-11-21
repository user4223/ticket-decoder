#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/input/api/include/Loader.h"
#include "lib/output/api/include/SinkManager.h"
#include "lib/input/detail/api/include/Reader.h"
#include "lib/output/detail/api/include/Writer.h"

#include "test/support/include/TestSupport.h"

using namespace input::api;
using namespace input::detail;
using namespace output::api;

std::filesystem::path getSourcePath()
{
    return ::test::support::get().getIOPath();
};

#ifdef WITH_PDF_INPUT
auto const expectedItems = 5;
#else
auto const expectedItems = 2;
#endif

class IoFixture
{
    ::test::support::TestSupport &testSupport = ::test::support::get();
    Loader loader = Loader(testSupport.getContext(), Reader::createAll(testSupport.getContext(), LoadOptions{}));
    std::filesystem::path const currentPath;
    output::api::SinkManager sinkManager;

public:
    IoFixture(std::optional<std::filesystem::path> destinationPath)
        : currentPath([this]()
                      { 
                auto cwd = std::filesystem::current_path();
                std::filesystem::current_path(testSupport.getExecutableFolderPath());
                return cwd; }()),
          sinkManager(output::api::SinkManager::create(testSupport.getContext())
                          .useDestinationPath(*destinationPath)
                          .build())
    {
    }

    ~IoFixture()
    {
        std::filesystem::current_path(currentPath);
    }

    Loader &getLoader() { return loader; }
    SinkManager &getSinkManager() { return sinkManager; }
};

TEST(Io, singleImageInputFileOutputDirectory)
{
    auto tempDirectory = std::filesystem::temp_directory_path();
    auto io = IoFixture(tempDirectory);
    EXPECT_EQ(1, io.getLoader().load(getSourcePath() / "minimal.png", [&](auto &&source)
                                     { 
                                    auto sink = io.getSinkManager().get(source);
                                    {
                                        auto const destinationPath = sink->write(source.getImage());
                                        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png_out.png", destinationPath);
                                        EXPECT_TRUE(std::filesystem::exists(destinationPath.string()));
                                    }
                                    {
                                        auto const destinationPath = sink->write(std::vector<std::uint8_t>{23});
                                        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png_out.raw", destinationPath);
                                        EXPECT_TRUE(std::filesystem::exists(destinationPath.string()));
                                    }
                                    {
                                        auto const destinationPath = sink->write(std::string{"{}"});
                                        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png_out.json", destinationPath);
                                        EXPECT_TRUE(std::filesystem::exists(destinationPath.string()));
                                    } }));
}

TEST(Io, multiImageInputFileOutputDirectory)
{
#ifndef WITH_PDF_INPUT
    GTEST_SKIP() << "PDF input support not compiled in";
#endif
    auto tempDirectory = std::filesystem::temp_directory_path();
    auto io = IoFixture(tempDirectory);
    auto paths = std::vector<std::filesystem::path>{};
    EXPECT_EQ(2, io.getLoader().load(getSourcePath() / "two-page.pdf", [&](auto &&source)
                                     { 
                                    auto sink = io.getSinkManager().get(source); 
                                    paths.push_back(sink->write(source.getImage())); }));

    EXPECT_EQ(2, paths.size());
    std::sort(paths.begin(), paths.end());
    EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_0_out.png", paths[0]);
    EXPECT_TRUE(std::filesystem::exists(paths[0].string()));
    EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_1_out.png", paths[1]);
    EXPECT_TRUE(std::filesystem::exists(paths[1].string()));
}

TEST(Io, inputDirectoryOutputDirectory)
{
    auto tempDirectory = std::filesystem::temp_directory_path();
    auto io = IoFixture(tempDirectory);
    auto paths = std::vector<std::filesystem::path>{};

    EXPECT_EQ(expectedItems, io.getLoader().load(getSourcePath(), [&](auto &&source)
                                                 {
                                    auto sink = io.getSinkManager().get(source); 
                                    paths.push_back(sink->write(source.getImage())); }));

    EXPECT_EQ(expectedItems, paths.size());
    std::sort(paths.begin(), paths.end());
#ifdef WITH_PDF_INPUT
    EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.jpg_out.png", paths[0]);
    EXPECT_TRUE(std::filesystem::exists(paths[0].string()));
    EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.pdf_out.png", paths[1]);
    EXPECT_TRUE(std::filesystem::exists(paths[1].string()));
    EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png_out.png", paths[2]);
    EXPECT_TRUE(std::filesystem::exists(paths[2].string()));
    EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_0_out.png", paths[3]);
    EXPECT_TRUE(std::filesystem::exists(paths[3].string()));
    EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_1_out.png", paths[4]);
    EXPECT_TRUE(std::filesystem::exists(paths[4].string()));
#else
    EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.jpg_out.png", paths[0]);
    EXPECT_TRUE(std::filesystem::exists(paths[0].string()));
    EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png_out.png", paths[1]);
    EXPECT_TRUE(std::filesystem::exists(paths[1].string()));
#endif
}
