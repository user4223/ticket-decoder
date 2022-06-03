
#include "../include/RecordInterpreter0080BL.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreter0080BL::RecordInterpreter0080BL(RecordHeader &&h) : header(std::move(h)) {}

Interpreter::Context &RecordInterpreter0080BL::interpret(Context &context)
{
  if (header.recordId.compare("0080BL") != 0 || header.recordVersion.compare("03") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.to_string());
  }

  return context;
}
