#pragma once

#include "InputElement.h"

#include <lib/utility/include/LoggingFwd.h>

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <map>
#include <functional>

namespace io::api
{
    class Reader;

    class LoadResult
    {
        struct Internal;
        std::shared_ptr<Internal> internal;

    public:
        LoadResult();
        LoadResult(std::vector<InputElement> elements);
        LoadResult(std::function<void(LoadResult &)>);
        LoadResult(LoadResult const &) = delete;
        LoadResult(LoadResult &&other) = default;
        LoadResult &operator=(LoadResult const &) = delete;
        LoadResult &operator=(LoadResult &&other) = default;

        bool inProgress() const;
        bool hasCompleted() const;

        void add(InputElement &&element);

        /* Returns the current count of elements, this might not be the final size of the result in
           case it has been created with loadAsync().
         */
        std::size_t size() const;

        InputElement get(std::size_t index) const;
    };

    class Loader
    {
    public:
        using ReaderMap = std::map<std::string, std::shared_ptr<Reader>>;

    private:
        ::utility::Logger logger;
        ReaderMap const readers;

    public:
        Loader(::utility::LoggerFactory &loggerFactory, std::vector<std::shared_ptr<Reader>> readers);

        LoadResult load(std::filesystem::path path) const;

        LoadResult loadAsync(std::filesystem::path path) const;

        /* Deprecated: Use Loader.load() instead of low-level scan and direct image read access
         */
        static std::vector<std::filesystem::path> scan(std::filesystem::path directory, std::vector<std::string> extensions);
    };
}
