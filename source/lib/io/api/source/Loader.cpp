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
#include <thread>

namespace io::api
{
    struct LoadResult::Internal
    {
        std::mutex mutex;
        std::vector<InputElement> elements;
        std::optional<std::future<void>> future;

        auto lock() { return std::lock_guard<std::mutex>(mutex); }
    };

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

    LoadResult::LoadResult(LoadResult &&other)
        : internal(other.internal)
    {
    }

    LoadResult &LoadResult::operator=(LoadResult &&other)
    {
        internal = other.internal;
        return *this;
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
            return true; // when there is no future anymore, it has completed for sure ;-)
        }
        auto completed = internal->future->wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        if (completed)
        {
            internal->future->get();
            internal->future.reset();
        }
        return completed;
    }

    bool LoadResult::waitForElementOrCompletion() const
    {
        if (size() > 0)
        {
            return true;
        }
        while (inProgress())
        {
            std::this_thread::yield();
            if (size() > 0)
            {
                return true;
            }
        }
        return size() > 0;
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
            elements.emplace_back(InputElement::fromFile(pathString, std::move(result.consumeImage())));
            return elements;
        }

        auto images = result.consumeImages();
        auto index = 0;
        std::for_each(std::begin(images), std::end(images), [&pathString, &elements, &index](auto &&image)
                      { elements.emplace_back(InputElement::fromFile(pathString, index++, std::move(image))); });
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

    size_t loadFile(Loader::ReaderMap const &readers, std::filesystem::path path, std::function<void(InputElement &&)> adder)
    {
        if (!std::filesystem::is_regular_file(path))
        {
            return 0;
        }
        auto reader = readers.find(Reader::normalizeExtension(path));
        if (reader == std::end(readers))
        {
            return 0;
        }
        auto elements = createInputElement(*(reader->second), path);
        std::for_each(std::begin(elements), std::end(elements), [&](auto &&element)
                      { adder(std::move(element)); });
        return elements.size();
    }

    size_t loadDirectory(Loader::ReaderMap const &readers, std::filesystem::path path, std::function<void(InputElement &&)> adder)
    {
        if (!std::filesystem::is_directory(path))
        {
            return 0;
        }
        auto const hiddenRegex = std::regex("[.].*", std::regex_constants::icase);
        auto counter = 0;
        for (auto const &entry : std::filesystem::recursive_directory_iterator(path))
        {
            auto const basename = entry.path().filename().string();
            if (std::regex_match(basename, hiddenRegex))
            {
                continue;
            }
            counter += loadFile(readers, entry.path(), adder);
        }
        return counter;
    }

    Loader::Loader(::utility::LoggerFactory &loggerFactory, std::vector<std::shared_ptr<Reader>> r)
        : logger(CREATE_LOGGER(loggerFactory)), readers(createExtensionReaderMap(std::move(r)))
    {
    }

    LoadResult Loader::load(std::filesystem::path path) const
    {
        auto result = std::vector<InputElement>{};
        load(path, [&result](auto &&element)
             { result.emplace_back(std::move(element)); });
        return LoadResult(std::move(result));
    }

    void Loader::load(std::filesystem::path path, std::function<void(InputElement &&)> handler) const
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error(std::string("Path to load input elements from does not exist: ") + path.string());
        }

        if (std::filesystem::is_regular_file(path))
        {
            auto const count = loadFile(readers, path, handler);
            LOG_DEBUG(logger) << "Loaded " << count << " image(s) from file synchronously: " << path;
        }
        else
        {
            auto const count = loadDirectory(readers, path, handler);
            LOG_INFO(logger) << "Loaded " << count << " image(s) from directory synchronously: " << path;
        }
    }

    LoadResult Loader::loadAsync(std::filesystem::path path) const
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error(std::string("Path to load input elements from does not exist: ") + path.string());
        }
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

                                LOG_INFO(logger) << "Loaded " << result.size() << " image(s) from directory asynchronously: " << path; });
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
