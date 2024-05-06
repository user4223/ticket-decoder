
#include <boost/python.hpp>

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/utility/include/Base64.h"
#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/Utility.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/dip/utility/include/PreProcessor.h"

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
    auto const interpreter = uic918::api::Interpreter::create(loggerFactory);
    auto const inputBytes = ::utility::base64::decode(base64RawData);
    auto const outputJson = interpreter->interpret(inputBytes, "", 3);
    if (!outputJson)
    {
        throw std::runtime_error("No UIC918 structured data found, version not matching or implemented, or interpretation failed");
    }
    return boost::python::str(*outputJson);
}

boost::python::dict decodeFile(std::string const &path)
{
    if (!io::api::utility::areFiles({path}))
    {
        PyErr_SetString(PyExc_RuntimeError, "Given path is not a file");
    }
    auto const readers = io::api::Reader::create(loggerFactory, io::api::ReadOptions{});
    auto const loader = io::api::Loader(loggerFactory, readers);
    auto parameters = dip::detection::api::Parameters{};
    auto const detector = dip::detection::api::Detector::create(loggerFactory, parameters);
    auto const preProcessor = dip::utility::PreProcessor::create(loggerFactory, 0, "11");
    auto const interpreter = uic918::api::Interpreter::create(loggerFactory);
    auto result = boost::python::dict{};
    loader.load(path, [&](auto &&inputElement)
                {
                    auto source = preProcessor.get(std::move(inputElement));
                    if (!source.isValid())
                    {
                        return;
                    }
                    auto detectionResult = detector->detect(source.getImage());
                    std::for_each(detectionResult.contours.begin(), detectionResult.contours.end(),
                              [&](auto const &contourDescriptor)
                              {
                                auto const decodingResult = barcode::api::Decoder::decode(loggerFactory, contourDescriptor, {false, false});
                                if (!decodingResult.isDecoded()) 
                                {
                                    return;
                                }
                                auto const interpretationResult = interpreter->interpret(decodingResult.payload, source.getAnnotation(), 3);
                                result[inputElement.getAnnotation()] = interpretationResult.value_or("{}");
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
