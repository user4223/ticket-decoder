
#include "../include/RecordU_FLEX.h"
#include "../include/Utility.h"

#include "../../api/include/Record.h"

#include "lib/uic918/gen/v1.3/UicRailTicketData.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

#include <sstream>
#include <string>
#include <optional>

namespace utility
{
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
  JsonBuilder &JsonBuilder::add(UTF8String_t *const field)
  {
    if (field == nullptr)
    {
      return *this;
    }
    return add(std::string((char *)field->buf, (std::size_t)field->size));
  }

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, OCTET_STRING_t const &field)
  {
    return add(name, std::string((char *)field.buf, (std::size_t)field.size));
  }

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, INTEGER_t const &field)
  {
    long value = 0;
    auto end = (char const *)(field.buf + field.size);
    ::asn_strtol_lim((char const *)field.buf, &end, &value);
    return add(name, value);
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
  static JsonBuilder toArray(SourceT const *const source, std::function<JsonBuilder(ElementT const &)> transformer)
  {
    auto builder = JsonBuilder::array();
    if (source == nullptr)
    {
      return builder;
    }
    for (auto index = 0; index < source->list.count; ++index)
    {
      builder.add(transformer(*(source->list.array[index])));
    }
    return builder;
  }

  template <typename SourceT>
  static JsonBuilder toArray(SourceT const *const source)
  {
    auto builder = JsonBuilder::array();
    if (source == nullptr)
    {
      return builder;
    }
    for (auto index = 0; index < source->list.count; ++index)
    {
      builder.add(source->list.array[index]);
    }
    return builder;
  }

  template <typename ElementT, typename SourceT>
  static JsonBuilder toObject(SourceT const *const source, std::function<JsonBuilder(ElementT const &)> transformer)
  {
    return source == nullptr ? JsonBuilder::object() : transformer(*source);
  }

  static std::optional<std::string> toString(ENUMERATED_t *const source)
  {
    return source == nullptr ? std::nullopt : std::make_optional(std::to_string((int)source->buf[0]));
  }
}
namespace uic918::detail
{
  std::optional<::utility::JsonBuilder> convertV13(::utility::Logger &logger, std::vector<std::uint8_t> const &bytes)
  {
    UicRailTicketData *decodedData = nullptr;
    auto asn_context = asn_codec_ctx_t{0};
    asn_context.max_stack_size = ASN__DEFAULT_STACK_MAX;
    auto const result = uper_decode_complete(&asn_context, &asn_DEF_UicRailTicketData, (void **)&decodedData, bytes.data(), bytes.size());
    if (result.code != RC_OK || decodedData == nullptr)
    {
      LOG_WARN(logger) << "ASN1 UPER decoding failed with: " << result.code;
      return std::nullopt;
    }

    auto recordJson = ::utility::JsonBuilder::object() // clang-format off
      .add("issuingDetail", ::utility::toObject<IssuingData>(&(decodedData->issuingDetail),
        [](auto const &issuingDetail)
        { 
          return ::utility::JsonBuilder::object()
            .add("securityProviderNum", issuingDetail.securityProviderNum)
            .add("securityProvider", issuingDetail.securityProviderIA5)
            .add("issuerNum", issuingDetail.issuerNum)
            .add("issuer", issuingDetail.issuerIA5)
            .add("issuingDate", utility::toIsoDate(&(issuingDetail.issuingYear), &(issuingDetail.issuingDay)))
            .add("issuingTime", issuingDetail.issuingTime) // No of minutes, 60 * 24 as a maximum
            .add("issuerName", issuingDetail.issuerName)
            .add("specimen", issuingDetail.specimen)
            .add("securePaperTicket", issuingDetail.securePaperTicket)
            .add("activated", issuingDetail.activated)
            .add("currency", issuingDetail.currency)
            .add("currencyFract", issuingDetail.currencyFract)
            .add("issuerPNR", issuingDetail.issuerPNR)
            .add("issuedOnTrainNum", issuingDetail.issuedOnTrainNum)
            .add("issuedOnTrain", issuingDetail.issuedOnTrainIA5)
            .add("issuedOnLine", issuingDetail.issuedOnLine)
            .add("pointOfSale", ::utility::toObject<GeoCoordinateType>(issuingDetail.pointOfSale,
              [](auto const& coordinate)
              {
                return ::utility::JsonBuilder::object()
                  .add("geoUnit", ::utility::toString(coordinate.geoUnit))
                  .add("coordinateSystem", ::utility::toString(coordinate.coordinateSystem))
                  .add("hemisphereLongitude", ::utility::toString(coordinate.hemisphereLongitude))
                  .add("hemisphereLatitude", ::utility::toString(coordinate.hemisphereLatitude))
                  .add("longitude", coordinate.longitude)
                  .add("latitude", coordinate.latitude)
                  .add("accuracy", ::utility::toString(coordinate.accuracy)); })); }))
      .add("travelerDetail", ::utility::toObject<TravelerData>(decodedData->travelerDetail, 
        [](auto const &travelerDetail) 
        {
          return ::utility::JsonBuilder::object()
            .add("preferredLanguage", travelerDetail.preferredLanguage)
            .add("groupName", travelerDetail.groupName)
            .add("traveler", ::utility::toArray<TravelerType>(travelerDetail.traveler,
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
        [&](auto const &documentData)
        {
          switch (documentData.ticket.present)
          {
          case DocumentData__ticket_PR_openTicket:
          {
            auto const openTicket = documentData.ticket.choice.openTicket;
            return ::utility::JsonBuilder::object()
              .add("token", ::utility::toObject<TokenType>(documentData.token,
                [](auto const& token)
                { 
                  return ::utility::JsonBuilder::object()
                    .add("tokenProviderNum", token.tokenProviderNum)
                    .add("tokenProvider", token.tokenProviderIA5)
                    .add("tokenSpecification", token.tokenSpecification)
                    .add("token", token.token); }))
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
                  [](auto const& region)
                  { return ::utility::JsonBuilder::object(); })) // TODO implement me
                .add("returnDescription", ::utility::toObject<ReturnRouteDescriptionType>(openTicket.returnDescription,
                  [](auto const& description)
                  { return ::utility::JsonBuilder::object()
                      .add("fromStationNum", description.fromStationNum)
                      .add("fromStationIA5", description.fromStationIA5)
                      .add("toStationNum", description.toStationNum)
                      .add("toStationIA5", description.toStationIA5)
                      .add("fromStationNameUTF8", description.fromStationNameUTF8)
                      .add("toStationNameUTF8", description.toStationNameUTF8)
                      .add("validReturnRegionDesc", description.validReturnRegionDesc)
                      .add("validRegion", ::utility::toArray<RegionalValidityType>(description.validReturnRegion, 
                        [](auto const& region)
                        { return ::utility::JsonBuilder::object(); })); }))  // TODO implement me
                .add("validFromDay", openTicket.validFromDay)                // Offset to issuing date
                .add("validFromTime", openTicket.validFromTime)
                .add("validFromUTCOffset", openTicket.validFromUTCOffset)    // * 15min
                .add("validUntilDay", openTicket.validUntilDay)              // Offset to validFrom date
                .add("validUntilTime", openTicket.validUntilTime)
                .add("validUntilUTCOffset", openTicket.validUntilUTCOffset)
                .add("activatedDay", ::utility::toArray(openTicket.activatedDay))
                .add("classCode", ::utility::toString(openTicket.classCode))
                .add("serviceLevel", openTicket.serviceLevel)
                .add("carrierNum", ::utility::toArray(openTicket.carrierNum))
                .add("carrier", ::utility::toArray(openTicket.carrierIA5))
                .add("includedServiceBrands", ::utility::toArray(openTicket.includedServiceBrands))
                .add("excludedServiceBrands", ::utility::toArray(openTicket.excludedServiceBrands))
                .add("tariffs", ::utility::toArray<TariffType>(openTicket.tariffs,
                  [](auto const &tariff)
                  {
                    return ::utility::JsonBuilder::object()
                      .add("numberOfPassengers", tariff.numberOfPassengers)
                      .add("passengerType", ::utility::toString(tariff.passengerType)) // adult: 0, senior: 1, child: 2, youth: 3, dog: 4, bicycle: 5, freeAddonPassenger: 6, freeAddonChild: 7
                      .add("ageBelow", tariff.ageBelow)
                      .add("ageAbove", tariff.ageAbove)
                      .add("travelerid", ::utility::toArray(tariff.travelerid))
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
                  [](auto const addOns)
                  {  return ::utility::JsonBuilder::object(); })) // TODO implement me
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

          LOG_WARN(logger) << "Unimplemented transport document data type: " << documentData.ticket.present;
          return ::utility::JsonBuilder::object();
        }));
    return std::make_optional(std::move(recordJson)); // clang-format on
  }

  std::optional<::utility::JsonBuilder> convertV30(::utility::Logger &logger, std::vector<std::uint8_t> const &bytes)
  {
    LOG_WARN(logger) << "Unimplemented decoding of U_FLEX v03 record";
    return ::utility::JsonBuilder::object();
  }

  static std::map<std::string, std::function<std::optional<::utility::JsonBuilder>(::utility::Logger &, std::vector<std::uint8_t> const &)>> const uflexInterpreterMap = {
      {std::string("13"), convertV13},
      {std::string("03"), convertV30}};

  RecordU_FLEX::RecordU_FLEX(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_FLEX", {"13", "03"});
  }

  Context RecordU_FLEX::interpret(Context &&context)
  {
    auto const asn1UperBytes = utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
    auto const uflexInterpreter = uflexInterpreterMap.at(header.recordVersion);
    auto recordJson = uflexInterpreter(logger, asn1UperBytes);

    if (!recordJson)
    {
      return std::move(context);
    }

    context.addRecord(api::Record(header.recordId, header.recordVersion, recordJson->build()));
    return std::move(context);
  }
}
