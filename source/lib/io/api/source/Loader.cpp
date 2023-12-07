#include "../include/Loader.h"

#include <regex>
#include <ostream>
#include <numeric>
#include <ranges>

namespace io::api
{
    std::map<std::string, std::shared_ptr<Reader>> createExtensionReaderMap(std::vector<std::shared_ptr<Reader>> readers)
    {
        auto map = std::map<std::string, std::shared_ptr<Reader>>{};
        std::for_each(std::begin(readers), std::end(readers), [&](auto reader)
                      { 
                        auto const extensions = reader->supportedExtensions(); 
                        std::for_each(std::begin(extensions), std::end(extensions), [&](auto extension)
                        {  map.emplace(extension, reader); }); });
        return map;
    }

    std::vector<InputElement> InputElement::create(Reader const &reader, std::filesystem::path path)
    {
        auto const pathString = path.string();
        auto result = reader.read(path);
        if (!result.isMultiPart())
        {
            return {InputElement{pathString, std::move(result.getImage())}};
        }
        auto elements = std::vector<InputElement>{};
        auto images = result.getImages();
        auto index = 0;
        std::for_each(std::begin(images), std::end(images), [&pathString, &elements, &index](auto image)
                      { elements.emplace_back(InputElement{pathString + "[" + std::to_string(index++) + "]", std::move(image)}); });
        return elements;
    }

    Loader::Loader(std::vector<std::shared_ptr<Reader>> r)
        : readers(createExtensionReaderMap(std::move(r)))
    {
    }

    std::vector<InputElement> Loader::load(std::filesystem::path path) const
    {
        if (!std::filesystem::exists(path))
        {
            return {};
        }
        if (std::filesystem::is_regular_file(path))
        {
            auto reader = readers.find(Reader::normalizeExtension(path));
            return reader == std::end(readers) ? std::vector<InputElement>{} : InputElement::create(*(reader->second), path);
        }
        if (!std::filesystem::is_directory(path))
        {
            return {};
        }

        auto result = std::vector<InputElement>{};
        auto const hiddenRegex = std::regex("[.].*", std::regex_constants::icase);
        for (auto const &entry : std::filesystem::recursive_directory_iterator(path))
        {
            auto const basename = entry.path().filename().string();
            if (!std::regex_match(basename, hiddenRegex))
            {
                auto reader = readers.find(Reader::normalizeExtension(entry.path()));
                if (reader != std::end(readers))
                {
                    auto elements = InputElement::create(*(reader->second), entry.path());
                    std::for_each(std::begin(elements), std::end(elements), [&](auto &&element)
                                  { result.emplace_back(std::move(element)); });
                }
            }
        }
        return result;
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
