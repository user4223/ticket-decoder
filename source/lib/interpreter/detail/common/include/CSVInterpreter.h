// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/infrastructure/include/Logger.h"

namespace interpreter::detail::common
{
    class CSVInterpreter : public common::Interpreter
    {
        infrastructure::Logger logger;

    public:
        CSVInterpreter(infrastructure::LoggerFactory &loggerFactory);

        virtual bool canInterpret(common::Context const &context) const override;

        virtual common::Context interpret(common::Context &&context) override;
    };
}
