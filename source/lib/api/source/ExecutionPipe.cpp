// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/ExecutionPipe.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logging.h"

namespace api {

    Element::Element(BarcodeBytes &&barcodeBytes, std::string annotation)
        : annotation(std::move(annotation)),
          image(std::nullopt),
          path(std::nullopt),
          decoded(std::move(barcodeBytes))
    {
    }

    Element::Element(ImageBytes &&imageBytes, std::string annotation)
        : annotation(std::move(annotation)),
          image(std::move(imageBytes)),
          path(std::nullopt),
          decoded(std::nullopt)
    {
    }

    Element::Element(ImageBytes &&imageBytes, std::filesystem::path p)
        : annotation(p.string()),
          image(std::move(imageBytes)),
          path(std::move(p)),
          decoded(std::nullopt)
    {
    }

    std::optional<Content> Element::get() const
    {
        return content;
    }

    struct ExecutionPipe::Internal {

        Supplier supplier;
        PreparatorTransformer preparatorTransformer = [](auto const& input) { return std::nullopt; };
        DetectorTransformer detectorTransformer = [](auto const& input) { return std::nullopt; };
        DecoderTransformer decoderTransformer = [](auto const& input) { return std::nullopt; };
        InterpreterTransformer interpreterTransformer = [](auto const& input) { return std::nullopt; };
        Consumer consumer = [](auto&& input) {};
        bool terminate = false;
    };

    ExecutionPipe::ExecutionPipe(infrastructure::Context &context)
        : internal(std::make_shared<Internal>())
    {
    }

    ExecutionPipe::~ExecutionPipe()
    {
    }

    Element ExecutionPipe::execute(Element &&e)
    {
        auto element = std::move(e);

        if (element.image)
        {
            auto preparatorResult = internal->preparatorTransformer(*element.image);
            if (preparatorResult)
            {
                element.image.emplace(std::move(*preparatorResult));
            }
            auto detectorResult = internal->detectorTransformer(*element.image);
            if (detectorResult)
            {
                element.image.emplace(std::move(*detectorResult));
            }
            auto decoderResult = internal->decoderTransformer(*element.image);
            if (decoderResult)
            {
                element.decoded.emplace(std::move(*decoderResult));
            }
        }

        if (element.decoded)
        {
            auto interpreterResult = internal->interpreterTransformer(*element.decoded);
            if (interpreterResult)
            {
                element.content.emplace(std::move(*interpreterResult));
            }
        }

        return std::move(element);
    }

    void ExecutionPipe::startExecution()
    {
    }

    void ExecutionPipe::shutdownExecution()
    {
        internal->terminate = true;
    }

    ExecutionPipe &ExecutionPipe::withSupplier(Supplier supplier)
    {
        internal->supplier = std::move(supplier);
        return *this;
    }

    ExecutionPipe &ExecutionPipe::withConsumer(Consumer consumer)
    {
        internal->consumer = std::move(consumer);
        return *this;
    }

    ExecutionPipe &ExecutionPipe::withPreparator(PreparatorTransformer transformer)
    {
        internal->preparatorTransformer = std::move(transformer);
        return *this;
    }

    ExecutionPipe &ExecutionPipe::withDetector(DetectorTransformer transformer)
    {
        internal->detectorTransformer = std::move(transformer);
        return *this;
    }

    ExecutionPipe &ExecutionPipe::withDecoder(DecoderTransformer transformer)
    {
        internal->decoderTransformer = std::move(transformer);
        return *this;
    }

    ExecutionPipe &ExecutionPipe::withInterpreter(InterpreterTransformer transformer)
    {
        internal->interpreterTransformer = std::move(transformer);
        return *this;
    }
}
