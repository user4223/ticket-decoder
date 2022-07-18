
#include "../include/RecordInterpreterU_FLEX.h"
#include "../include/Utility.h"

#include "../gen/UicRailTicketData.h"
#include "../gen/per_decoder.h"

#include <sstream>
#include <string>

RecordInterpreterU_FLEX::RecordInterpreterU_FLEX(RecordHeader &&h) : AbstractRecord(std::move(h))
{
  header.ensure("U_FLEX", {"13"});
}

Context &RecordInterpreterU_FLEX::interpret(Context &context)
{
  auto const asn1UperBytes = Utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));

  UicRailTicketData *decodedData = nullptr;
  auto asn_context = asn_codec_ctx_t{0};
  asn_context.max_stack_size = ASN__DEFAULT_STACK_MAX;
  auto const result = uper_decode_complete(&asn_context, &asn_DEF_UicRailTicketData, (void **)&decodedData, asn1UperBytes.data(), asn1UperBytes.size());
  if (decodedData == nullptr)
  {
    return context;
  }

  auto const prefix = std::string("U_FLEX.");
  context.addField(prefix + "firstName", std::string((char *)decodedData->travelerDetail->traveler->list.array[0]->firstName->buf));
  context.addField(prefix + "lastName", std::string((char *)decodedData->travelerDetail->traveler->list.array[0]->lastName->buf));
  context.addField(prefix + "tarifDescription", std::string((char *)decodedData->transportDocument->list.array[0]->ticket.choice.openTicket.tariffs->list.array[0]->tariffDesc->buf));

  return context;
}
