// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <filesystem>
#include <functional>

namespace api 
{
    class ImageBytes
    {
        std::size_t columns;
        std::size_t lines;
        std::size_t feed;
        std::vector<std::uint8_t> bytes;
    };

    class BarcodeBytes
    {
        std::vector<std::uint8_t> bytes;
    };

    class Content
    {
        std::string json;
    };

    class Element {

        std::string annotation;
        std::optional<std::filesystem::path> path;
        std::optional<ImageBytes> image;
        std::optional<BarcodeBytes> decoded;
        std::optional<Content> content;

    public:
        static Element create(ImageBytes &&imageBytes, std::string annotation);

        static Element create(ImageBytes &&imageBytes, std::filesystem::path path);
    };

    class ExecutionPipe {

        void supply(Element &&element);

        ExecutionPipe &withPreparator(std::function<ImageBytes(ImageBytes const &)> transformer);

        ExecutionPipe &withDetector(std::function<std::optional<ImageBytes>(ImageBytes const &)> transformer);

        ExecutionPipe &withDecoder(std::function<std::optional<BarcodeBytes>(ImageBytes const &)> transformer);

        ExecutionPipe &withInterpreter(std::function<std::optional<Content>(BarcodeBytes const &)> transformer);

        ExecutionPipe &withConsumer(std::function<void(Element &&)> consumer);
    };
}
