
#include "../include/DecoderFacade.h"

#include "lib/utility/include/Logging.h"

namespace api
{
    struct DecoderFacade::Internal
    {
    };

    DecoderFacade::DecoderFacade(::utility::LoggerFactory &loggerFactory)
        : internal(std::make_shared<Internal>())
    {
    }
}
