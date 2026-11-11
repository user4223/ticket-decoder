// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <lib/infrastructure/include/ContextFwd.h>

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>

namespace api
{
    struct ImageBytes
    {
        std::size_t columns;
        std::size_t lines;
        std::size_t feed;
        std::vector<std::uint8_t> bytes;
    };

    struct BarcodeBytes
    {
        std::vector<std::uint8_t> bytes;
    };

    struct Content
    {
        std::string json;
    };

    class Element
    {
        ImageBytes image;
        std::string annotation;
        std::optional<std::filesystem::path> path;
        std::optional<BarcodeBytes> decoded;
        std::optional<Content> content;

        friend class ExecutionPipe;

    public:
        Element(ImageBytes &&imageBytes, std::string annotation);

        Element(ImageBytes &&imageBytes, std::filesystem::path path);

        std::optional<Content> get() const;
    };

    class ExecutionPipe {

        struct Internal;
        std::shared_ptr<Internal> internal;

        using PreparatorTransformer = std::function<std::optional<ImageBytes>(ImageBytes const &image)>;
        using DetectorTransformer = std::function<std::optional<ImageBytes>(ImageBytes const &image)>;
        using DecoderTransformer = std::function<std::optional<BarcodeBytes>(ImageBytes const &image)>;
        using InterpreterTransformer = std::function<std::optional<Content>(BarcodeBytes const &barcode)>;
        using Consumer = std::function<void(Element &&element)>;

        Element processSync(Element &&element);

    public:
        ExecutionPipe(infrastructure::Context &context);

        void supply(Element &&element);

        ExecutionPipe &withPreparator(PreparatorTransformer transformer);

        ExecutionPipe &withDetector(DetectorTransformer transformer);

        ExecutionPipe &withDecoder(DecoderTransformer transformer);

        ExecutionPipe &withInterpreter(InterpreterTransformer transformer);

        ExecutionPipe &withConsumer(Consumer consumer);
    };
}
