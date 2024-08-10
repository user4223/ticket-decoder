
#include "../include/Context.h"

#include "lib/utility/include/Logging.h"

#include <memory>

namespace infrastructure
{
    struct Internal
    {
        ::utility::LoggerFactory loggerFactory;
        ::utility::DebugController debugController;
    };

    static std::unique_ptr<Internal> internal;

    void initialize(Internal *i)
    {
        if (internal.get() != nullptr)
        {
            throw std::runtime_error("Global context object cannot be initialized twice");
        }
        internal.reset(i);
    }

    Context::Context()
    {
        initialize(new Internal{::utility::LoggerFactory::createLazy(false)});
    }

    Context::Context(::utility::LoggerFactory loggerFactory)
    {
        initialize(new Internal{std::move(loggerFactory)});
    }

    Context::Context(::utility::LoggerFactory loggerFactory, ::utility::DebugController debugController)
    {
        initialize(new Internal{std::move(loggerFactory), std::move(debugController)});
    }

    Context::~Context()
    {
        internal.reset();
    }

    ::utility::LoggerFactory &Context::getLoggerFactory()
    {
        return internal->loggerFactory;
    }

    ::utility::DebugController &Context::getDebugController()
    {
        return internal->debugController;
    }
}
