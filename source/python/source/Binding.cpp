
#include <boost/python.hpp>

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/utility/include/Base64.h"
#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/Utility.h"
#include "lib/api/include/DecoderFacade.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/dip/filtering/include/PreProcessor.h"

#include "lib/barcode/api/include/Decoder.h"

#include <exception>
#include <sstream>
#include <map>

static auto loggerFactory = ::utility::LoggerFactory::createLazy(false);

void errorTranslator(std::exception const &x)
{
    auto message = std::stringstream();
    message << "Decoding failed with: " << x.what();
    PyErr_SetString(PyExc_RuntimeError, message.str().c_str());
}

boost::python::str decodeUIC918(std::string const &base64RawData)
{
    auto decoderFacade = ::api::DecoderFacade(loggerFactory);
    return boost::python::str(decoderFacade.decodeUIC918(base64RawData));
}

boost::python::list decodeFile(std::string const &path)
{
    if (!io::api::utility::areFiles({path}))
    {
        PyErr_SetString(PyExc_RuntimeError, "Given path is not a file");
    }
    auto const readers = io::api::Reader::create(loggerFactory);
    auto const loader = io::api::Loader(loggerFactory, readers);
    auto const detector = dip::detection::api::Detector::create(loggerFactory, dip::detection::api::Detector::Type::NOP_FORWARDER);
    auto const preProcessor = dip::filtering::PreProcessor::create(loggerFactory, 0, "11");
    auto const interpreter = uic918::api::Interpreter::create(loggerFactory);
    auto result = boost::python::list{};
    loader.load(path, [&](auto &&inputElement)
                {
                    auto source = preProcessor.get(std::move(inputElement));
                    if (!source.isValid())
                    {
                        return;
                    }
                    auto const barcodes = detector->detect(source.getImage());
                    barcodes.for_each([&](auto const &contourDescriptor)
                                    {
                                        auto const binaryContent = barcode::api::Decoder::decode(loggerFactory, contourDescriptor, {false, false});
                                        if (!binaryContent.isDecoded())
                                        {
                                            return;
                                        }
                                        auto const jsonContent = interpreter->interpret(binaryContent.payload, source.getAnnotation(), 3);
                                        result.append(jsonContent.value_or("{}"));
                                    }); });
    return result;
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
