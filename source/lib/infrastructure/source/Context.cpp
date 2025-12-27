// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Context.h"
#include "../include/Logging.h"

#include <memory>

namespace infrastructure
{
    struct Internal
    {
        LoggerFactory loggerFactory;
        DebugController debugController;
        Logger logger;

        Internal(LoggerFactory lf, DebugController dc)
            : loggerFactory(std::move(lf)),
              debugController(std::move(dc)),
              logger(CREATE_LOGGER(loggerFactory))
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
        initialize(new Internal(LoggerFactory::createLazy(false), DebugController()));
    }

    Context::Context(LoggerFactory loggerFactory)
    {
        initialize(new Internal(std::move(loggerFactory), DebugController()));
    }

    Context::Context(LoggerFactory loggerFactory, DebugController debugController)
    {
        initialize(new Internal(std::move(loggerFactory), std::move(debugController)));
    }

    Context::~Context()
    {
        internal.reset(nullptr);
    }

    LoggerFactory &Context::getLoggerFactory()
    {
        return internal->loggerFactory;
    }

    DebugController &Context::getDebugController()
    {
        return internal->debugController;
    }
}
