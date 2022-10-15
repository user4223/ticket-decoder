
#include "../include/RecordU_FLEX.h"
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

  RecordU_FLEX::RecordU_FLEX(RecordHeader &&h)
      : AbstractRecord(std::move(h))
  {
    header.ensure("U_FLEX", {"13"});
  }

  Context &RecordU_FLEX::interpret(Context &context)
  {
    auto const asn1UperBytes = utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));

    UicRailTicketData *decodedData = nullptr;
    auto asn_context = asn_codec_ctx_t{0};
    asn_context.max_stack_size = ASN__DEFAULT_STACK_MAX;
    auto const result = uper_decode_complete(&asn_context, &asn_DEF_UicRailTicketData, (void **)&decodedData, asn1UperBytes.data(), asn1UperBytes.size());
    if (result.code != RC_OK || decodedData == nullptr)
    {
      return context;
    }

    auto recordJson = ::utility::JsonBuilder::object() // clang-format off
      .add("travelerDetail", ::utility::toObject<TravelerData>(decodedData->travelerDetail, 
        [](auto const &travelerDetail) 
        {
          return ::utility::JsonBuilder::object()
            .add("travelers", ::utility::toArray<TravelerType>(travelerDetail.traveler,
              [](auto const &traveler)
              { 
                return ::utility::JsonBuilder::object()
                  .add("firstName", traveler.firstName)
                  .add("secondName", traveler.secondName)
                  .add("lastName", traveler.lastName)
                  .add("idCard", traveler.idCard)
                  .add("passportId", traveler.passportId)
                  .add("title", traveler.title)
                  .add("gender", ::utility::toString(traveler.gender))
                  .add("customerId", traveler.customerIdIA5)
                  .add("customerIdNum", traveler.customerIdNum)
                  .add("dateOfBirth", utility::toIsoDate(traveler.yearOfBirth, traveler.dayOfBirth))
                  .add("ticketHolder", traveler.ticketHolder)
                  .add("passengerType", ::utility::toString(traveler.passengerType))
                  .add("passengerWithReducedMobility", traveler.passengerWithReducedMobility)
                  .add("countryOfResidence", traveler.countryOfResidence)
                  .add("countryOfPassport", traveler.countryOfPassport)
                  .add("countryOfIdCard", traveler.countryOfIdCard)
                  .add("status", ::utility::toArray<CustomerStatusType>(traveler.status,
                    [](auto const& status){
                      return ::utility::JsonBuilder::object()
                        .add("statusProvider", status.statusProviderIA5)                        
                        .add("customerStatus", status.customerStatus)
                        .add("customerStatusDescr", status.customerStatusDescr); })); })); }))
                  
      .add("transportDocuments", ::utility::toArray<DocumentData>(decodedData->transportDocument,
        [](auto const &documentData)
        {
          switch (documentData.ticket.present)
          {
          case DocumentData__ticket_PR_openTicket:
          {
            auto const openTicket = documentData.ticket.choice.openTicket;
            return ::utility::JsonBuilder::object()
              .add("openTicket", ::utility::JsonBuilder::object()
                .add("referenceNum", openTicket.referenceNum)
                .add("reference", openTicket.referenceIA5)
                .add("productOwnerNum", openTicket.productOwnerNum)
                .add("productOwner", openTicket.productOwnerIA5)
                .add("productIdNum", openTicket.productIdNum)
                .add("productId", openTicket.productIdIA5)
                .add("extIssuerId", openTicket.extIssuerId)
                .add("issuerAutorizationId", openTicket.issuerAutorizationId)
                .add("returnIncluded", openTicket.returnIncluded)
                .add("stationCodeTable", ::utility::toString(openTicket.stationCodeTable))
                .add("fromStationNum", openTicket.fromStationNum)
                .add("fromStation", openTicket.fromStationIA5)
                .add("toStationNum", openTicket.toStationNum)
                .add("toStation", openTicket.toStationIA5)
                .add("fromStationName", openTicket.fromStationNameUTF8)
                .add("toStationName", openTicket.toStationNameUTF8)
                .add("validRegionDesc", openTicket.validRegionDesc)
                .add("validRegion", ::utility::toArray<RegionalValidityType>(openTicket.validRegion, 
                  [](auto const& region)                        // TODO implement
                  { return ::utility::JsonBuilder::object(); }))
                .add("returnDescription", ::utility::toObject<ReturnRouteDescriptionType>(openTicket.returnDescription,
                  [](auto const& description)                   // TODO implement
                  { return ::utility::JsonBuilder::object(); }))
                .add("validFromDay", openTicket.validFromDay)   // Offset to issuing date
                .add("validFromTime", openTicket.validFromTime)
                .add("validFromUTCOffset", openTicket.validFromUTCOffset) // * 15min
                .add("validUntilDay", openTicket.validUntilDay) // Offset to validFrom date
                .add("validUntilTime", openTicket.validUntilTime)
                .add("validUntilUTCOffset", openTicket.validUntilUTCOffset)
                .add("activatedDay", ::utility::toArray<long>(openTicket.activatedDay,
                  [](auto const day)                            // TODO implement
                  {  return ::utility::JsonBuilder::object(); }))
                .add("classCode", ::utility::toString(openTicket.classCode))
                .add("serviceLevel", openTicket.serviceLevel)
                .add("carrierNum", ::utility::toArray<long>(openTicket.carrierNum,
                  [](auto const carrierNo)                      // TODO implement
                  {  return ::utility::JsonBuilder::object(); }))
                .add("carrier", ::utility::toArray<IA5String_t>(openTicket.carrierIA5,
                  [](auto const carrier)                        // TODO implement
                  {  return ::utility::JsonBuilder::object(); }))
                .add("includedServiceBrands", ::utility::toArray<long>(openTicket.includedServiceBrands,
                  [](auto const includedBrands)                 // TODO implement
                  {  return ::utility::JsonBuilder::object(); }))
                .add("excludedServiceBrands", ::utility::toArray<long>(openTicket.excludedServiceBrands,
                  [](auto const excludedBrands)                 // TODO implement
                  {  return ::utility::JsonBuilder::object(); }))
                .add("tariffs", ::utility::toArray<TariffType>(openTicket.tariffs,
                  [](auto const &tariff)
                  {
                    return ::utility::JsonBuilder::object()
                      .add("numberOfPassengers", tariff.numberOfPassengers)
                      .add("passengerType", tariff.passengerType)
                      .add("ageBelow", tariff.ageBelow)
                      .add("ageAbove", tariff.ageAbove)
                      .add("travelerid", ::utility::toArray<long>(tariff.travelerid,
                        [](auto const excludedBrands)            // TODO implement
                        {  return ::utility::JsonBuilder::object(); }))
                      .add("restrictedToCountryOfResidence", tariff.restrictedToCountryOfResidence)
                      .add("restrictedToRouteSection", ::utility::toObject<RouteSectionType>(tariff.restrictedToRouteSection, 
                        [](auto const& route)
                        { return ::utility::JsonBuilder::object()
                            .add("stationCodeTable", ::utility::toString(route.stationCodeTable))
                            .add("fromStationNum", route.fromStationNum)
                            .add("fromStation", route.fromStationIA5)
                            .add("toStationNum", route.toStationNum)
                            .add("toStation", route.toStationIA5)
                            .add("fromStationName", route.fromStationNameUTF8)
                            .add("toStationName", route.toStationNameUTF8); }))
                      .add("seriesDataDetails", ::utility::toObject<SeriesDetailType>(tariff.seriesDataDetails, 
                        [](auto const& series)
                        { return ::utility::JsonBuilder::object()
                            .add("supplyingCarrier", series.supplyingCarrier)
                            .add("offerIdentification", series.offerIdentification)
                            .add("series", series.series); }))
                      .add("tariffIdNum", tariff.tariffIdNum)
                      .add("tariffId", tariff.tariffIdIA5)
                      .add("tariffDesc", tariff.tariffDesc)
                      .add("reductionCard", ::utility::toArray<CardReferenceType>(tariff.reductionCard,
                        [](auto const& card)
                        { return ::utility::JsonBuilder::object()
                            .add("cardIssuerNum", card.cardIssuerNum)
                            .add("cardIssuer", card.cardIssuerIA5)
                            .add("cardIdNum", card.cardIdNum)
                            .add("cardId", card.cardIdIA5)
                            .add("cardName", card.cardName)
                            .add("cardType", card.cardType)
                            .add("leadingCardIdNum", card.leadingCardIdNum)
                            .add("leadingCardId", card.leadingCardIdIA5)
                            .add("trailingCardIdNum", card.trailingCardIdNum)
                            .add("trailingCardId", card.trailingCardIdIA5); })); }))
                .add("price", openTicket.price))
                .add("vatDetail", ::utility::toArray<VatDetailType>(openTicket.vatDetail,
                  [](auto const &detail)
                  { return ::utility::JsonBuilder::object()
                      .add("country", detail.country)
                      .add("percentage", detail.percentage)
                      .add("amount", detail.amount)
                      .add("vatId", detail.vatId); }))
                .add("infoText", openTicket.infoText)
                .add("includedAddOns", ::utility::toArray<IncludedOpenTicketType>(openTicket.includedAddOns,
                  [](auto const addOns)                         // TODO implement
                  {  return ::utility::JsonBuilder::object(); }))
                .add("luggage", ::utility::toObject<LuggageRestrictionType>(openTicket.luggage,
                  [](auto const& luggage)
                  { return ::utility::JsonBuilder::object()
                      .add("maxHandLuggagePieces", luggage.maxHandLuggagePieces)
                      .add("maxNonHandLuggagePieces", luggage.maxNonHandLuggagePieces)
                      .add("registeredLuggage", ::utility::toArray<RegisteredLuggageType>(luggage.registeredLuggage, 
                        [](auto const& registered)
                        { return ::utility::JsonBuilder::object()
                            .add("maxWeight", registered.maxWeight)
                            .add("maxSize", registered.maxSize); })); }));
          } break;
          default: break;
          }
          return ::utility::JsonBuilder::object(); })); // clang-format on

    return context.addRecord(api::Record(header.recordId, header.recordVersion, recordJson.build()));
  }
}
