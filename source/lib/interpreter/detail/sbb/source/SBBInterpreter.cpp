
#include "../include/SBBInterpreter.h"

#include "lib/utility/include/Logging.h"

namespace interpreter::detail::sbb
{
    Interpreter::TypeIdType SBBInterpreter::getTypeId()
    {
        return {0x0A, 0xC8, 0x01}; // This is just 3 bytes of protobuf preable and not finally indicating it's an SBB ticket, but for now....
    }

    SBBInterpreter::SBBInterpreter(::utility::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    Context SBBInterpreter::interpret(Context &&context)
    {
        LOG_WARN(logger) << "Unsupported SBB barcode detected";
        return std::move(context);
    }
}
