
#include "../include/RecordInterpreterU_FLEX.h"
#include "../include/Utility.h"
#include "../../api/include/Record.h"

#include "../gen/UicRailTicketData.h"
#include "../gen/per_decoder.h"

#include "lib/utility/include/JsonBuilder.h"

#include <sstream>
#include <string>
#include <optional>

namespace utility
{
  using json = nlohmann::json;

  /* Explicit specializations for some ASN1 specific types */
  template <>
  JsonBuilder &JsonBuilder::add(std::string name, UTF8String_t *const field)
  {
    if (field == nullptr)
    {
      return *this;
    }
    return add(name, std::string((char *)field->buf, (std::size_t)field->size));
  }

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, INTEGER_t *const field)
  {
    if (field == nullptr)
    {
      return *this;
    }
    long value = 0;
    auto end = (char const *)(field->buf + field->size);
    ::asn_strtol_lim((char const *)field->buf, &end, &value);
    return add(name, value);
  }

  template <typename ElementT, typename SourceT>
  static std::optional<json> toArray(SourceT const *const source, std::function<JsonBuilder(ElementT const &)> consumer)
  {
    if (source == nullptr)
    {
      return std::nullopt;
    }
    json value = json::array();
    for (auto index = 0; index < source->list.count; ++index)
    {
      auto const entry = source->list.array[index];
      value.insert(value.end(), consumer(*entry).value);
    }
    return value.empty() ? std::nullopt : std::make_optional(std::move(value));
  }

  template <typename ElementT, typename SourceT>
  static std::optional<json> toObject(SourceT const *const source, std::function<JsonBuilder(ElementT const &)> consumer)
  {
    if (source == nullptr)
    {
      return std::nullopt;
    }
    auto value = consumer(*source).value;
    return value.empty() ? std::nullopt : std::make_optional(std::move(value));
  }

  static std::optional<std::string> toString(ENUMERATED_t *const source)
  {
    if (source == nullptr)
    {
      return std::nullopt;
    }
    return std::make_optional(std::to_string((int)source->buf[0]));
  }
}

namespace uic918::detail
{

  RecordInterpreterU_FLEX::RecordInterpreterU_FLEX(RecordHeader &&h)
      : AbstractRecord(std::move(h))
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
    if (result.code != RC_OK || decodedData == nullptr)
    {
      return context;
    }

    auto json = utility::JsonBuilder::object() // clang-format off
    .add("travelerDetail", utility::toObject<TravelerData>(decodedData->travelerDetail, 
      [](auto const &travelerDetail) 
      {
        return utility::JsonBuilder::object()
          .add("travelers", utility::toArray<TravelerType>(travelerDetail.traveler,
            [](auto const &traveler)
            { 
              return utility::JsonBuilder::object()
                .add("firstName", traveler.firstName)
                .add("secondName", traveler.secondName)
                .add("lastName", traveler.lastName)
                .add("dateOfBirth", Utility::toIsoDate(traveler.yearOfBirth, traveler.dayOfBirth)); })); }))
    .add("transportDocuments", utility::toArray<DocumentData>(decodedData->transportDocument,
      [](auto const &documentData)
      {
        switch (documentData.ticket.present)
        {
        case DocumentData__ticket_PR_openTicket:
        {
          auto const openTicket = documentData.ticket.choice.openTicket;
          return utility::JsonBuilder::object()
            .add("openTicket", utility::JsonBuilder::object()
              .add("referenceIA5", openTicket.referenceIA5)
              .add("classCode", utility::toString(openTicket.classCode))
              .add("tariffs", utility::toArray<TariffType>(openTicket.tariffs,
                [](auto const &tariff)
                {
                  return utility::JsonBuilder::object()
                    .add("numberOfPassengers", tariff.numberOfPassengers)
                    .add("tariffDesc", tariff.tariffDesc); }))
              .add("price", openTicket.price));
        } break;
        default: break;
        }
        return utility::JsonBuilder::object(); }))
    .build(); // clang-format on

    auto record = api::Record(header.recordId, header.recordVersion, std::move(json));
    return context.addRecord(std::move(record));
  }
}
