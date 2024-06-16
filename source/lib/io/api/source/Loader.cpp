#include "../include/Loader.h"
#include "../include/Reader.h"

#include "lib/utility/include/Logging.h"

#include <regex>
#include <ostream>
#include <numeric>
#include <ranges>

namespace io::api
{
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

    size_t Loader::load(std::filesystem::path path, std::function<void(InputElement &&)> handler) const
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error(std::string("Path to load input elements from does not exist: ") + path.string());
        }

        auto count = 0;
        if (std::filesystem::is_regular_file(path))
        {
            count = loadFile(readers, path, handler);
            LOG_DEBUG(logger) << "Loaded " << count << " image(s) from file synchronously: " << path;
        }
        else
        {
            count = loadDirectory(readers, path, handler);
            LOG_INFO(logger) << "Loaded " << count << " image(s) from directory synchronously: " << path;
        }
        return count;
    }

    LoadResult Loader::loadAsync(std::filesystem::path path) const
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error(std::string("Path to load input elements from does not exist: ") + path.string());
        }
        if (std::filesystem::is_regular_file(path))
        {
            return LoadResult([logger = this->logger, readers = this->readers, path](auto adder)
                              {
                                  auto const count = loadFile(readers, path, [adder](auto &&element)
                                                    {   adder(std::move(element)); });

                                  LOG_DEBUG(logger) << "Loaded " << count << " image(s) from file asynchronously: " << path; });
        }
        else
        {
            return LoadResult([logger = this->logger, readers = this->readers, path](auto adder)
                              {
                                  auto const count = loadDirectory(readers, path, [adder](auto &&element)
                                                     {  adder(std::move(element)); });

                                  LOG_INFO(logger) << "Loaded " << count << " image(s) from directory asynchronously: " << path; });
        }
    }

    std::future<size_t> Loader::loadAsync(std::filesystem::path path, std::function<void(InputElement &&)> handler) const
    {

        if (std::filesystem::is_regular_file(path))
        {
            return std::async(std::launch::async, [logger = this->logger, readers = this->readers, path, handler]()
                              {
                                auto const count = loadFile(readers, path, handler);
                                LOG_DEBUG(logger) << "Loaded " << count << " image(s) from file asynchronously: " << path;
                                return count; });
        }
        else
        {
            return std::async(std::launch::async, [logger = this->logger, readers = this->readers, path, handler]()
                              {
                                auto const count = loadDirectory(readers, path, handler);
                                LOG_INFO(logger) << "Loaded " << count << " image(s) from directory asynchronously: " << path;
                                return count; });
        }
    }

    /* Deprecated: Use Loader.load() instead of low-level scan and direct image read access
     */
    std::vector<std::filesystem::path> scan(std::filesystem::path directory, std::vector<std::string> extensions)
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
