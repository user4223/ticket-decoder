
#include "../include/RecordInterpreterU_FLEX.h"
#include "../include/Utility.h"

#include "../gen/UicRailTicketData.h"
#include "../gen/per_decoder.h"

#include "nlohmann/json.hpp"

#include <sstream>
#include <string>

using json = nlohmann::json;

RecordInterpreterU_FLEX::RecordInterpreterU_FLEX(RecordHeader &&h) : AbstractRecord(std::move(h))
{
  header.ensure("U_FLEX", {"13"});
}

template <typename ElementT, typename ListT>
json forEach(ListT const &list, std::function<json(ElementT const &)> consumer)
{
  auto result = json::array();
  for (auto index = 0; index < list.count; ++index)
  {
    auto const entry = list.array[index];
    result.insert(result.end(), consumer(*entry));
  }
  return result;
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

  auto jsonRecord = json::object();
  auto const travelerDetail = decodedData->travelerDetail;
  if (travelerDetail != nullptr)
  {
    auto jsonTravelerDetail = json::object();
    auto const traveler = travelerDetail->traveler;
    if (traveler != nullptr)
    {
      jsonTravelerDetail["travelers"] = forEach<TravelerType>(
          traveler->list,
          [&](auto const &entry)
          {
            auto jsonEntry = json::object();
            if (entry.firstName != nullptr)
              jsonEntry["firstName"] = std::string((char *)entry.firstName->buf);
            if (entry.lastName != nullptr)
              jsonEntry["lastName"] = std::string((char *)entry.lastName->buf);
            return jsonEntry;
          });
    }
    jsonRecord["travelerDetail"] = jsonTravelerDetail;
  }

  auto const transportDocument = decodedData->transportDocument;
  if (transportDocument != nullptr)
  {
    auto jsonTransportDocuments = json::array();
    for (auto index = 0; index < transportDocument->list.count; ++index)
    {
      auto const transportDocumentEntry = transportDocument->list.array[index];
      auto jsonTransportDocumentEntry = json::object();
      {
        auto const ticket = transportDocumentEntry->ticket;
        auto jsonTicket = json::object();
        switch (ticket.present)
        {
        case DocumentData__ticket_PR_openTicket:
        {
          auto const openTicket = ticket.choice.openTicket;
          if (openTicket.referenceIA5 != nullptr)
            jsonTicket["referenceIA5"] = std::string((char *)openTicket.referenceIA5->buf);
          if (openTicket.classCode != nullptr)
            jsonTicket["classCode"] = std::to_string((int)openTicket.classCode->buf[0]);

          auto const tariffs = openTicket.tariffs;
          if (tariffs != nullptr)
          {
            auto jsonTariffs = json::array();
            for (auto k = 0; k < tariffs->list.count; ++k)
            {
              auto const tariffEntry = tariffs->list.array[k];
              auto jsonTariffEntry = json::object();

              if (tariffEntry->numberOfPassengers != nullptr)
                jsonTariffEntry["numberOfPassengers"] = *tariffEntry->numberOfPassengers;
              if (tariffEntry->tariffDesc != nullptr)
                jsonTariffEntry["tariffDesc"] = std::string((char *)tariffEntry->tariffDesc->buf);

              jsonTariffs.insert(jsonTariffs.end(), jsonTariffEntry);
            }
            jsonTicket["tariffs"] = jsonTariffs;
          }
        }
        break;
        default:
          break;
        }
        jsonTransportDocumentEntry["ticket"] = jsonTicket;
      }
      jsonTransportDocuments.insert(jsonTransportDocuments.end(), jsonTransportDocumentEntry);
    }
    jsonRecord["transportDocuments"] = jsonTransportDocuments;
  }

  context.addRecord("U_FLEX", jsonRecord.dump());
  return context;
}
