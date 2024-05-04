
#include <boost/python.hpp>

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/utility/include/Base64.h"
#include "lib/utility/include/Logging.h"

#include <exception>
#include <sstream>

static auto loggerFactory = ::utility::LoggerFactory::createLazy(false);

void errorTranslator(std::exception const &x)
{
    auto message = std::stringstream();
    message << "Decoding failed with: " << x.what();
    PyErr_SetString(PyExc_RuntimeError, message.str().c_str());
}

std::string decodeUIC918(std::string const &base64RawData)
{
    auto const interpreter = uic918::api::Interpreter::create(loggerFactory);
    auto const inputBytes = ::utility::base64::decode(base64RawData);
    auto const outputJson = interpreter->interpret(inputBytes, 3);
    if (!outputJson)
    {
        throw std::runtime_error("No UIC918 structured data found, version not matching or implemented, or interpretation failed");
    }
    return *outputJson;
}

std::map<std::string, std::string> decodeFiles(std::vector<std::string> paths)
{
    return {};
}

std::map<std::string, std::string> decodeFile(std::string path)
{
    return decodeFiles({path});
}

BOOST_PYTHON_MODULE(ticket_decoder)
{
    Py_Initialize();
    boost::python::register_exception_translator<std::exception>(errorTranslator);
    boost::python::def("decode_uic918", decodeUIC918);
    boost::python::def("decode_files", decodeFiles);
    boost::python::def("decode_file", decodeFile);
}
