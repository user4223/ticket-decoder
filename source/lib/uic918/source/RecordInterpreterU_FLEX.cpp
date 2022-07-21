
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

struct Builder
{
  json value;

  static Builder object()
  {
    return Builder{json::object()};
  }

  template <typename T>
  Builder &add(std::string name, T const *field);

  template <>
  Builder &add(std::string name, UTF8String_t const *field)
  {
    if (field != nullptr)
      value[name] = json((char *)field->buf);
    return *this;
  }

  template <>
  Builder &add(std::string name, long const *field)
  {
    if (field != nullptr)
      value[name] = json(*field);
    return *this;
  }

  Builder &add(std::string name, json const &subTree)
  {
    value[name] = subTree;
    return *this;
  }
};

template <typename SourceT>
struct ArrayBuilder
{
  SourceT const *const source;
  json value;

  ArrayBuilder<SourceT> &ifPresent(std::function<void(json &&)> adder)
  {
    if (!value.empty())
    {
      adder(std::move(value));
    }
    return *this;
  }

  template <typename ElementT>
  ArrayBuilder<SourceT> &transformEach(std::function<json(ElementT const &)> consumer)
  {
    if (source == nullptr)
    {
      return *this;
    }
    for (auto index = 0; index < source->list.count; ++index)
    {
      auto const entry = source->list.array[index];
      value.insert(value.end(), consumer(*entry));
    }
    return *this;
  }
};

template <typename T>
static ArrayBuilder<T> listOf(T const *const source)
{
  return ArrayBuilder<T>{source, json::array()};
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
    listOf(travelerDetail->traveler)
        .transformEach<TravelerType>(
            [](auto const &entry)
            { auto travelerBuilder = Builder::object();
              travelerBuilder.add("firstName", entry.firstName);
              travelerBuilder.add("secondName", entry.secondName);
              travelerBuilder.add("lastName", entry.lastName);
              return travelerBuilder.value; })
        .ifPresent(
            [&](auto &&result)
            { jsonTravelerDetail["travelers"] = result; });

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
        auto ticketBuilder = Builder::object();
        switch (ticket.present)
        {
        case DocumentData__ticket_PR_openTicket:
        {
          auto const openTicket = ticket.choice.openTicket;
          ticketBuilder.add("referenceIA5", openTicket.referenceIA5);
          ticketBuilder.add("classCode", std::to_string((int)openTicket.classCode->buf[0]));
          listOf(openTicket.tariffs)
              .transformEach<TariffType>(
                  [](auto const &entry)
                  {
                    auto tariffBuilder = Builder::object();
                    tariffBuilder.add("numberOfPassengers", entry.numberOfPassengers);
                    tariffBuilder.add("tariffDesc", entry.tariffDesc);
                    return tariffBuilder.value; })
              .ifPresent(
                  [&](auto &&result)
                  { ticketBuilder.add("tariffs", result); });
        }
        break;
        default:
          break;
        }
        jsonTransportDocumentEntry["ticket"] = ticketBuilder.value;
      }
      jsonTransportDocuments.insert(jsonTransportDocuments.end(), jsonTransportDocumentEntry);
    }
    jsonRecord["transportDocuments"] = jsonTransportDocuments;
  }

  context.addRecord("U_FLEX", jsonRecord.dump());
  return context;
}
