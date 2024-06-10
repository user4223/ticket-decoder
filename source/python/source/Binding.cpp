
#include <boost/python.hpp>

#include "lib/utility/include/Logging.h"

#include "lib/api/include/DecoderFacade.h"

#include <exception>
#include <sstream>

static auto loggerFactory = ::utility::LoggerFactory::createLazy(false);

class Memoizer
{
    static std::unique_ptr<Memoizer> decoderFacade;

    api::DecoderFacade facade = api::DecoderFacade::create(loggerFactory)
                                    .withFailOnInterpretationError(true)
                                    .build();

public:
    static api::DecoderFacade &get()
    {
        if (!decoderFacade)
        {
            decoderFacade = std::make_unique<Memoizer>();
        }
        return decoderFacade->facade;
    }
};
std::unique_ptr<Memoizer> Memoizer::decoderFacade;

void errorTranslator(std::exception const &x)
{
    auto message = std::stringstream();
    message << "Decoding failed with: " << x.what();
    PyErr_SetString(PyExc_RuntimeError, message.str().c_str());
}

boost::python::str decodeUIC918(std::string const &base64RawData)
{
    return boost::python::str(Memoizer::get().decodeRawBase64ToJson(base64RawData));
}

boost::python::list decodeFile(std::string const &path)
{
    auto const result = Memoizer::get().decodeFileToJson(path);
    auto list = boost::python::list();
    std::for_each(result.begin(), result.end(), [&](auto &&item)
                  { list.append(std::move(item)); });
    return list;
}

BOOST_PYTHON_MODULE(ticket_decoder)
{
    Py_Initialize();
    boost::python::register_exception_translator<std::exception>(errorTranslator);
    boost::python::def("decode_uic918", decodeUIC918, "Decode base64-encoded raw UIC918 data into structured json",
                       boost::python::args("Base64-encoded UIC918 raw data"));
    boost::python::def("decode_file", decodeFile, "Decode Aztec-Code and containing UIC918 data into structured json",
                       boost::python::args("Path to image or PDF file containing Aztec-Codes"));
}
