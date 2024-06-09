#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/SinkManager.h"

#include "test/support/include/Loader.h"

namespace io::api
{
    static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

    std::filesystem::path getSourcePath()
    {
        return support::Loader::getExecutableFolderPath() / "etc" / "io";
    };

    class IoFixture
    {
        io::api::Loader loader = io::api::Loader(loggerFactory, io::api::Reader::create(loggerFactory, io::api::ReaderOptions{}));
        std::filesystem::path const currentPath;
        io::api::SinkManager sinkManager;

    public:
        IoFixture(std::optional<std::filesystem::path> destinationPath)
            : currentPath([]()
                          { 
                auto cwd = std::filesystem::current_path();
                std::filesystem::current_path(support::Loader::getExecutableFolderPath());
                return cwd; }()),
              sinkManager(io::api::SinkManager::create()
                              .useDestination(*destinationPath)
                              .build())
        {
        }

        ~IoFixture()
        {
            std::filesystem::current_path(currentPath);
        }

        io::api::Loader &getLoader() { return loader; }
        io::api::SinkManager &getSinkManager() { return sinkManager; }
    };

    TEST(Io, singleImageInputFileOutputDirectory)
    {
        auto tempDirectory = std::filesystem::temp_directory_path();
        auto io = IoFixture(tempDirectory);
        EXPECT_EQ(1, io.getLoader().load(getSourcePath() / "minimal.png", [&](auto &&source)
                                         { 
                                    auto sink = io.getSinkManager().get(source);
                                    {
                                        auto const destinationPath = sink.write(source.getImage());
                                        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png", destinationPath);
                                        EXPECT_TRUE(std::filesystem::exists(destinationPath.string()));
                                    }
                                    {
                                        auto const destinationPath = sink.write(std::vector<std::uint8_t>{23});
                                        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png.raw", destinationPath);
                                        EXPECT_TRUE(std::filesystem::exists(destinationPath.string()));
                                    }
                                    {
                                        auto const destinationPath = sink.write(std::string{"{}"});
                                        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png.json", destinationPath);
                                        EXPECT_TRUE(std::filesystem::exists(destinationPath.string()));
                                    } }));
    }

    TEST(Io, multiImageInputFileOutputDirectory)
    {
        auto tempDirectory = std::filesystem::temp_directory_path();
        auto io = IoFixture(tempDirectory);
        auto paths = std::vector<std::filesystem::path>{};
        EXPECT_EQ(2, io.getLoader().load(getSourcePath() / "two-page.pdf", [&](auto &&source)
                                         { 
                                    auto sink = io.getSinkManager().get(source); 
                                    paths.push_back(sink.write(source.getImage())); }));

        EXPECT_EQ(2, paths.size());
        std::sort(paths.begin(), paths.end());
        EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_0.png", paths[0]);
        EXPECT_TRUE(std::filesystem::exists(paths[0].string()));
        EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_1.png", paths[1]);
        EXPECT_TRUE(std::filesystem::exists(paths[1].string()));
    }

    TEST(Io, inputDirectoryOutputDirectory)
    {
        auto tempDirectory = std::filesystem::temp_directory_path();
        auto io = IoFixture(tempDirectory);
        auto paths = std::vector<std::filesystem::path>{};
        EXPECT_EQ(5, io.getLoader().load(getSourcePath(), [&](auto &&source)
                                         { 
                                    auto sink = io.getSinkManager().get(source); 
                                    paths.push_back(sink.write(source.getImage())); }));

        EXPECT_EQ(5, paths.size());
        std::sort(paths.begin(), paths.end());
        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.jpg.png", paths[0]);
        EXPECT_TRUE(std::filesystem::exists(paths[0].string()));
        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.pdf.png", paths[1]);
        EXPECT_TRUE(std::filesystem::exists(paths[1].string()));
        EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png", paths[2]);
        EXPECT_TRUE(std::filesystem::exists(paths[2].string()));
        EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_0.png", paths[3]);
        EXPECT_TRUE(std::filesystem::exists(paths[3].string()));
        EXPECT_EQ(tempDirectory / "etc" / "io" / "two-page.pdf_1.png", paths[4]);
        EXPECT_TRUE(std::filesystem::exists(paths[4].string()));
    }
}
