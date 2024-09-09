
#include <boost/python.hpp>

#include "lib/infrastructure/include/Context.h"

#include "lib/api/include/DecoderFacade.h"

#include <exception>
#include <sstream>
#include <memory>

class DecoderFacadeWrapper
{
    struct Instance
    {
        infrastructure::Context context;
        api::DecoderFacade facade;

        Instance()
            : context(::utility::LoggerFactory::createLazy(false)),
              facade(api::DecoderFacade::create(context)
                         .withFailOnInterpreterError(true)
                         .build())
        {
        }
    };

    std::shared_ptr<Instance> instance;

    api::DecoderFacade &get() { return instance->facade; }

public:
    DecoderFacadeWrapper() : instance(std::make_shared<Instance>()) {}
    DecoderFacadeWrapper(DecoderFacadeWrapper const &) = default;
    DecoderFacadeWrapper &operator=(DecoderFacadeWrapper const &) = default;

    boost::python::str decodeUIC918(std::string const &base64RawData)
    {
        return boost::python::str(get().decodeRawBase64ToJson(base64RawData));
    }

    boost::python::list decodeFile(std::string const &path)
    {
        auto const result = get().decodeImageFilesToJson(path);
        auto list = boost::python::list();
        std::for_each(result.begin(), result.end(), [&](auto &&item)
                      { list.append(boost::python::make_tuple(item.first, item.second)); });
        return list;
    }
};

void errorTranslator(std::exception const &x)
{
    auto message = std::stringstream();
    message << "Decoding failed with: " << x.what();
    PyErr_SetString(PyExc_RuntimeError, message.str().c_str());
}

BOOST_PYTHON_MODULE(ticket_decoder)
{
    Py_Initialize();

    boost::python::register_exception_translator<std::exception>(errorTranslator);

    boost::python::class_<DecoderFacadeWrapper>("DecoderFacade", boost::python::init())
        .def("decode_uic918", &DecoderFacadeWrapper::decodeUIC918, "Decode base64-encoded raw UIC918 data into structured json",
             boost::python::args("Base64-encoded UIC918 raw data"), boost::python::return_by_value())
        .def("decode_file", &DecoderFacadeWrapper::decodeFile, "Decode Aztec-Code and containing UIC918 data into structured json",
             boost::python::args("Path to image or PDF file containing Aztec-Codes"), boost::python::return_by_value());
}
