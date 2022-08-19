
#include "../include/RecordInterpreterU_FLEX.h"
#include "../include/Utility.h"

#include "../gen/UicRailTicketData.h"
#include "../gen/per_decoder.h"

#include "../api/include/Record.h"

#include "nlohmann/json.hpp"

#include <sstream>
#include <string>
#include <optional>

using json = nlohmann::json;
namespace uic918::detail
{
  struct Builder
  {
    json value;

    static Builder object()
    {
      return Builder{json::object()};
    }

    template <typename T>
    Builder &add(std::string name, T const *field);

    Builder &add(std::string name, json const &subTree)
    {
      if (!subTree.empty())
      {
        value[name] = subTree;
      }
      return *this;
    }

    Builder &add(std::string name, Builder const &subTreeBuilder)
    {
      return add(name, subTreeBuilder.value);
    }

    template <typename T>
    Builder &add(std::string name, std::optional<T> const &value)
    {
      if (value.has_value())
      {
        add(name, *value);
      }
      return *this;
    }

    template <typename ElementT, typename SourceT>
    static std::optional<json> toArray(SourceT const *const source, std::function<Builder(ElementT const &)> consumer)
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
    static std::optional<json> toObject(SourceT const *const source, std::function<Builder(ElementT const &)> consumer)
    {
      if (source == nullptr)
      {
        return std::nullopt;
      }
      auto value = consumer(*source).value;
      return value.empty() ? std::nullopt : std::make_optional(std::move(value));
    }

    template <typename ElementT>
    static std::optional<std::string> enumToString(ElementT const *const source)
    {
      if (source == nullptr)
      {
        return std::nullopt;
      }
      return std::make_optional(std::to_string((int)source->buf[0]));
    }

    template <typename ElementT>
    static std::optional<int> toLong(ElementT const *const source)
    {
      if (source == nullptr)
      {
        return std::nullopt;
      }
      long value = 0;
      auto end = (char const *)(source->buf + source->size);
      ::asn_strtol_lim((char const *)source->buf, &end, &value);
      return std::make_optional(value);
    }

    std::string build()
    {
      return value.dump();
    }
  };

  template <>
  Builder &Builder::add(std::string name, UTF8String_t const *field)
  {
    if (field != nullptr)
      value[name] = json((char *)field->buf);
    return *this;
  }

  template <>
  Builder &Builder::add(std::string name, long const *field)
  {
    if (field != nullptr)
      value[name] = json(*field);
    return *this;
  }

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

    auto json = Builder::object() // clang-format off
    .add("travelerDetail", Builder::toObject<TravelerData>(decodedData->travelerDetail, 
      [](auto const &travelerDetail) 
      {
        return Builder::object()
          .add("travelers", Builder::toArray<TravelerType>(travelerDetail.traveler, 
            [](auto const &traveler)
            { 
              return Builder::object()
                .add("firstName", traveler.firstName)
                .add("secondName", traveler.secondName)
                .add("lastName", traveler.lastName)
                .add("dateOfBirth", Utility::toIsoDate(traveler.yearOfBirth, traveler.dayOfBirth)); })); }))
    .add("transportDocuments", Builder::toArray<DocumentData>(decodedData->transportDocument, 
      [](auto const &documentData)
      {
        switch (documentData.ticket.present)
        {
        case DocumentData__ticket_PR_openTicket:
        {
          auto const openTicket = documentData.ticket.choice.openTicket;
          return Builder::object()
            .add("openTicket", Builder::object()
              .add("referenceIA5", openTicket.referenceIA5)
              .add("classCode", Builder::enumToString(openTicket.classCode))
              .add("tariffs", Builder::toArray<TariffType>(openTicket.tariffs, 
                [](auto const &tariff)
                {
                  return Builder::object()
                    .add("numberOfPassengers", tariff.numberOfPassengers)
                    .add("tariffDesc", tariff.tariffDesc); }))
              .add("price", Builder::toLong(openTicket.price)));
        } break;
        default: break;
        }
        return Builder::object(); }))
    .build(); // clang-format on

    auto record = Record(header.recordId, header.recordVersion, std::move(json));
    return context.addRecord(std::move(record));
  }
}
