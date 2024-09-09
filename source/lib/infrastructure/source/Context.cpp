
#include "../include/Context.h"

#include "lib/utility/include/Logging.h"

#include <memory>

namespace infrastructure
{
    struct Internal
    {
        ::utility::LoggerFactory loggerFactory;
        ::utility::DebugController debugController;
        //::utility::Logger logger;

        Internal(::utility::LoggerFactory lf, ::utility::DebugController dc)
            : loggerFactory(std::move(lf)),
              debugController(std::move(dc))
        // logger(CREATE_LOGGER(loggerFactory))
        {
        }

        ~Internal()
        {
            // LOG_DEBUG(logger) << "Context destroyed";
        }
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
        initialize(new Internal(::utility::LoggerFactory::createLazy(false), ::utility::DebugController()));
    }

    Context::Context(::utility::LoggerFactory loggerFactory)
    {
        initialize(new Internal(std::move(loggerFactory), ::utility::DebugController()));
    }

    Context::Context(::utility::LoggerFactory loggerFactory, ::utility::DebugController debugController)
    {
        initialize(new Internal(std::move(loggerFactory), std::move(debugController)));
    }

    Context::~Context()
    {
        // internal.reset(nullptr);
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
