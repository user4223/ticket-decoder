#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/utility/include/Logger.h"

namespace interpreter::detail::sbb
{
    class SBBInterpreter : public Interpreter
    {
        ::utility::Logger logger;

    public:
        static TypeIdType getTypeId();

        SBBInterpreter(::utility::LoggerFactory &loggerFactory);

        virtual Context interpret(Context &&context) override;
    };
}
