
#include <boost/python.hpp>

char const *doHelloWorld()
{
    return "Hello World";
}

BOOST_PYTHON_MODULE(ticket_decoder)
{
    Py_Initialize();
    boost::python::def("helloWorld", doHelloWorld);
}
