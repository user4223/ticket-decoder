#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <memory>

namespace api
{
    class DecoderFacade
    {
        struct Internal;

        std::shared_ptr<Internal> internal;

    public:
        DecoderFacade(::utility::LoggerFactory &loggerFactory);
    };
}
