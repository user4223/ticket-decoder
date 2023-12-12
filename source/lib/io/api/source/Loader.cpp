#include "../include/Loader.h"
#include "../include/Reader.h"

#include "lib/utility/include/Logging.h"

#include <regex>
#include <ostream>
#include <numeric>
#include <ranges>
#include <mutex>
#include <optional>
#include <future>

namespace io::api
{
    struct LoadResult::Internal
    {
        std::mutex mutex;
        std::vector<InputElement> elements;
        std::optional<std::future<void>> future;

        auto lock() { return std::lock_guard<std::mutex>(mutex); }
    };

    LoadResult::LoadResult()
        : internal(std::make_shared<Internal>())
    {
    }

    LoadResult::LoadResult(std::vector<InputElement> e)
        : internal(std::make_shared<Internal>())
    {
        internal->elements = std::move(e);
    }

    LoadResult::LoadResult(std::function<void(LoadResult &)> supplier)
        : internal(std::make_shared<Internal>())
    {
        internal->future = std::async(std::launch::async, [this, supplier]() mutable
                                      { supplier(*this); });
    }

    bool LoadResult::inProgress() const
    {
        return !hasCompleted();
    }

    bool LoadResult::hasCompleted() const
    {
        auto guard = internal->lock();
        if (!internal->future.has_value())
        {
            return true;
        }
        auto completed = internal->future->wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        if (completed)
        {
            internal->future->get();
            internal->future.reset();
        }
        return completed;
    }

    void LoadResult::add(InputElement &&element)
    {
        auto guard = internal->lock();
        internal->elements.emplace_back(std::move(element));
    }

    std::size_t LoadResult::size() const
    {
        auto guard = internal->lock();
        return internal->elements.size();
    }

    InputElement LoadResult::get(std::size_t index) const
    {
        auto guard = internal->lock();
        if (index >= internal->elements.size())
        {
            throw std::runtime_error("Cannot access element " + std::to_string(index) + " of container with size: " + std::to_string(internal->elements.size()));
        }
        return internal->elements[index];
    }

    std::vector<InputElement> createInputElement(Reader const &reader, std::filesystem::path path)
    {
        auto elements = std::vector<InputElement>{};
        auto const pathString = path.string();
        auto result = reader.read(path);
        if (!result.isMultiPart())
        {
            elements.emplace_back(InputElement{pathString, std::move(result.consumeImage())});
            return elements;
        }

        auto images = result.consumeImages();
        auto index = 0;
        std::for_each(std::begin(images), std::end(images), [&pathString, &elements, &index](auto &&image)
                      { elements.emplace_back(InputElement{pathString + "[" + std::to_string(index++) + "]", std::move(image)}); });
        return elements;
    }

    Loader::ReaderMap createExtensionReaderMap(std::vector<std::shared_ptr<Reader>> readers)
    {
        auto map = std::map<std::string, std::shared_ptr<Reader>>{};
        std::for_each(std::begin(readers), std::end(readers), [&](auto reader)
                      { 
                        auto const extensions = reader->supportedExtensions(); 
                        std::for_each(std::begin(extensions), std::end(extensions), [&](auto extension)
                        {  map.emplace(extension, reader); }); });
        return map;
    }

    void loadFile(Loader::ReaderMap const &readers, std::filesystem::path path, std::function<void(InputElement &&)> adder)
    {
        if (!std::filesystem::is_regular_file(path))
        {
            return;
        }
        auto reader = readers.find(Reader::normalizeExtension(path));
        if (reader == std::end(readers))
        {
            return;
        }
        auto elements = createInputElement(*(reader->second), path);
        std::for_each(std::begin(elements), std::end(elements), [&](auto &&element)
                      { adder(std::move(element)); });
    }

    void loadDirectory(Loader::ReaderMap const &readers, std::filesystem::path path, std::function<void(InputElement &&)> adder)
    {
        if (!std::filesystem::is_directory(path))
        {
            return;
        }
        auto const hiddenRegex = std::regex("[.].*", std::regex_constants::icase);
        for (auto const &entry : std::filesystem::recursive_directory_iterator(path))
        {
            auto const basename = entry.path().filename().string();
            if (std::regex_match(basename, hiddenRegex))
            {
                continue;
            }
            loadFile(readers, entry.path(), adder);
        }
    }

    Loader::Loader(::utility::LoggerFactory &loggerFactory, std::vector<std::shared_ptr<Reader>> r)
        : logger(CREATE_LOGGER(loggerFactory)), readers(createExtensionReaderMap(std::move(r)))
    {
    }

    LoadResult Loader::load(std::filesystem::path path) const
    {
        auto result = std::vector<InputElement>{};
        if (std::filesystem::is_regular_file(path))
        {
            loadFile(readers, path, [&result](auto &&element)
                     { result.emplace_back(std::move(element)); });

            LOG_DEBUG(logger) << "Loaded " << result.size() << " image(s) from file synchronously: " << path;
        }
        else
        {
            loadDirectory(readers, path, [&result](auto &&element)
                          { result.emplace_back(std::move(element)); });

            LOG_DEBUG(logger) << "Loaded " << result.size() << " image(s) from directory synchronously: " << path;
        }
        return LoadResult(std::move(result));
    }

    LoadResult Loader::loadAsync(std::filesystem::path path) const
    {
        if (std::filesystem::is_regular_file(path))
        {
            return LoadResult([logger = this->logger, readers = this->readers, path](auto &result)
                              {
                                loadFile(readers, path, [&result](auto &&element)
                                         { result.add(std::move(element)); });

                                LOG_DEBUG(logger) << "Loaded " << result.size() << " image(s) from file asynchronously: " << path; });
        }
        else
        {
            return LoadResult([logger = this->logger, readers = this->readers, path](auto &result)
                              {
                                loadDirectory(readers, path, [&result](auto &&element)
                                              { result.add(std::move(element)); });

                                LOG_DEBUG(logger) << "Loaded " << result.size() << " image(s) from directory asynchronously: " << path; });
        }
    }

    std::vector<std::filesystem::path> Loader::scan(std::filesystem::path directory, std::vector<std::string> extensions)
    {
        if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
        {
            if (std::filesystem::is_regular_file(directory))
            {
                return {directory};
            }
            return {};
        }

        auto result = std::vector<std::filesystem::path>{};
        auto joinedExtensions = std::stringstream();
        std::copy(extensions.begin(), extensions.end(), std::ostream_iterator<std::string>(joinedExtensions, "|"));
        auto const extensionRegex = std::regex("[.](" + joinedExtensions.str() + ")", std::regex_constants::icase);
        auto const hiddenRegex = std::regex("[.].*", std::regex_constants::icase);
        for (auto const &entry : std::filesystem::recursive_directory_iterator(directory))
        {
            auto const extension = entry.path().extension().string();
            auto const basename = entry.path().filename().string();
            if (std::regex_match(extension, extensionRegex) && !std::regex_match(basename, hiddenRegex))
            {
                result.push_back(entry.path());
            }
        }
        return result;
    }
}
