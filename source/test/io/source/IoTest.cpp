#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/SinkManager.h"

#include "test/support/include/Loader.h"

namespace io::api
{
    std::filesystem::path getSourcePath()
    {
        return support::Loader::getExecutableFolderPath() / "etc" / "io";
    };

    class IoFixture
    {
        utility::LoggerFactory loggerFactory = utility::LoggerFactory::create();
        io::api::Loader loader = io::api::Loader(loggerFactory, io::api::Reader::create(loggerFactory, io::api::ReadOptions{}));
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

    TEST(Io, inputFileOutputDirectory)
    {
        auto tempDirectory = std::filesystem::temp_directory_path();
        auto io = IoFixture(tempDirectory);
        EXPECT_EQ(1, io.getLoader().load(getSourcePath() / "minimal.png", [&](auto &&source)
                                         { 
                                    auto sink = io.getSinkManager().get(source); 
                                    auto const destinationPath = sink.write(source.getImage());
                                    EXPECT_EQ(tempDirectory / "etc" / "io" / "minimal.png", destinationPath);
                                    EXPECT_TRUE(std::filesystem::exists(destinationPath.string())); }));
    }
}
