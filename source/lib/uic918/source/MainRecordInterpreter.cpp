
#include "../include/MainRecordInterpreter.h"
#include "../include/Utility.h"

Interpreter::Context MainRecordInterpreter::interpret(Context &&context)
{
  if (context.uncompressedMessage.size() < 8)
  {
    return std::move(context);
  }
  auto position = Interpreter::BytesType::const_iterator(context.uncompressedMessage.begin());
  auto const recordId = Utility::getAlphanumeric(position, 6);
  auto const recordVersion = Utility::getAlphanumeric(position, 2);
  if (recordId.compare("U_HEAD") != 0 || recordVersion.compare("01") != 0)
  {
    return std::move(context);
  }
  context.output.insert(std::make_pair("mainRecord.recordId", recordId));
  context.output.insert(std::make_pair("mainRecord.recordVersion", recordVersion));
  context.output.insert(std::make_pair("mainRecord.companyCode", Utility::getAlphanumeric(position, 4)));
  context.output.insert(std::make_pair("mainRecord.unambiguousTicketKey", Utility::getAlphanumeric(position, 20)));

  return std::move(context);
}
