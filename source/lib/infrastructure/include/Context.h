#pragma once

#include "lib/utility/include/LoggerFactory.h"
#include "lib/utility/include/DebugController.h"

namespace infrastructure
{
    class Context
    {
    public:
        Context();
        Context(::utility::LoggerFactory loggerFactory);
        Context(::utility::LoggerFactory loggerFactory, ::utility::DebugController debugController);
        ~Context();

        Context(Context const &) = delete;
        Context(Context &&) = delete;
        Context &operator=(Context const &) = delete;
        Context &operator=(Context &&) = delete;

        ::utility::LoggerFactory &getLoggerFactory();

        ::utility::DebugController &getDebugController();
    };
}
