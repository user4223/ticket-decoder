// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "LoggerFactory.h"
#include "DebugController.h"

namespace infrastructure
{
    class Context
    {
    public:
        Context();
        Context(LoggerFactory loggerFactory);
        Context(LoggerFactory loggerFactory, DebugController debugController);
        ~Context();

        Context(Context const &) = delete;
        Context(Context &&) = delete;
        Context &operator=(Context const &) = delete;
        Context &operator=(Context &&) = delete;

        LoggerFactory &getLoggerFactory();

        DebugController &getDebugController();
    };
}
