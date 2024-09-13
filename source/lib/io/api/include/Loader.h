#pragma once

#include "InputElement.h"
#include "LoadResult.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logger.h"

#include <filesystem>
#include <vector>
#include <map>
#include <functional>
#include <future>

namespace io::api
{
    class Reader;

    class Loader
    {
    public:
        using ReaderMap = std::map<std::string, std::shared_ptr<Reader>>;

    private:
        ::utility::Logger logger;
        ReaderMap const readers;

    public:
        Loader(infrastructure::Context &context, std::vector<std::shared_ptr<Reader>> readers);

        LoadResult load(std::filesystem::path path) const;

        size_t load(std::filesystem::path path, std::function<void(InputElement &&)> handler) const;

        LoadResult loadAsync(std::filesystem::path path) const;

        std::future<size_t> loadAsync(std::filesystem::path path, std::function<void(InputElement &&)> handler) const;
    };
}
