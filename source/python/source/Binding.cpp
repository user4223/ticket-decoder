
#include <boost/python.hpp>

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/utility/include/Base64.h"
#include "lib/utility/include/Logging.h"

static auto loggerFactory = ::utility::LoggerFactory::createLazy(false);

std::string decodeUIC918(std::string const &base64RawData)
{
    auto const interpreter = uic918::api::Interpreter::create(loggerFactory);
    auto const inputBytes = ::utility::base64::decode(base64RawData);
    auto const outputJson = interpreter->interpret(inputBytes, 3);
    return outputJson.value_or("{}");
}

BOOST_PYTHON_MODULE(ticket_decoder)
{
    Py_Initialize();
    boost::python::def("decode_uic918", decodeUIC918);
}
