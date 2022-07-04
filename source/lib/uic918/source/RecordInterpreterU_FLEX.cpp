
#include "../include/RecordInterpreterU_FLEX.h"
#include "../include/Utility.h"

#include "sstream"

RecordInterpreterU_FLEX::RecordInterpreterU_FLEX(RecordHeader &&h) : header(std::move(h))
{
  header.ensure("U_FLEX", {"13"});
}

Context &RecordInterpreterU_FLEX::interpret(Context &context)
{
  Utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
  return context;
}
