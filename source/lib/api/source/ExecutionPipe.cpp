// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/ExecutionPipe.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logging.h"

namespace api {

    Element::Element(ImageBytes &&imageBytes, std::string annotation)
        : image(std::move(imageBytes)),
          annotation(std::move(annotation)),
          path(std::nullopt)
    {
    }

    Element::Element(ImageBytes &&imageBytes, std::filesystem::path p)
        : image(std::move(imageBytes)),
          annotation(p.string()),
          path(std::move(p))
    {
    }

    std::optional<Content> Element::get() const
    {
        return content;
    }

    struct ExecutionPipe::Internal {

        PreparatorTransformer preparatorTransformer = [](auto const& input) { return std::nullopt; };
        DetectorTransformer detectorTransformer = [](auto const& input) { return std::nullopt; };
        DecoderTransformer decoderTransformer = [](auto const& input) { return std::nullopt; };
        InterpreterTransformer interpreterTransformer = [](auto const& input) { return std::nullopt; };
        Consumer consumer = [](auto&& input) {};
    };

    Element ExecutionPipe::processSync(Element &&e)
    {
        auto element = std::move(e);
        auto preparatorResult = internal->preparatorTransformer(element.image);
        if (preparatorResult)
        {
            element.image = std::move(*preparatorResult);
        }
        auto detectorResult = internal->detectorTransformer(element.image);
        if (detectorResult)
        {
            element.image = std::move(*detectorResult);
        }
        auto decoderResult = internal->decoderTransformer(element.image);
        if (decoderResult)
        {
            element.decoded.emplace(std::move(*decoderResult));

            auto interpreterResult = internal->interpreterTransformer(*element.decoded);
            if (interpreterResult)
            {
                element.content.emplace(std::move(*interpreterResult));
            }
        }
        return std::move(element);
    }

    ExecutionPipe::ExecutionPipe(infrastructure::Context &context)
        : internal(std::make_shared<Internal>())
    {
    }

    void ExecutionPipe::supply(Element &&element)
    {
        internal->consumer(processSync(std::move(element)));
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

    ExecutionPipe &ExecutionPipe::withConsumer(Consumer consumer)
    {
        internal->consumer = std::move(consumer);
        return *this;
    }
}
