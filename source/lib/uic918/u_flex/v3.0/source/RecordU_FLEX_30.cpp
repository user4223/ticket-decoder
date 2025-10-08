
#include "../include/RecordU_FLEX_30.h"

#include "lib/utility/include/Logging.h"

#include "../../include/Utility.h"
#include "../../include/JsonSupport.h"

#include "../gen/UicRailTicketData.h"

namespace utility
{
  /* Explicit specializations for some ASN1 specific types */

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
  JsonBuilder &JsonBuilder::add(std::string name, UTF8String_t const &field)
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

  static std::optional<std::string> toString(ENUMERATED_t *const source)
  {
    return source == nullptr ? std::nullopt : std::make_optional(std::to_string((int)source->buf[0]));
  }
}

namespace uic918::u_flex30
{
  std::optional<::utility::JsonBuilder> convert(::utility::Logger &logger, std::vector<std::uint8_t> const &bytes)
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

    auto const issuingDate = std::make_pair(decodedData->issuingDetail.issuingYear, decodedData->issuingDetail.issuingDay);

    auto recordJson = ::utility::JsonBuilder::object() // clang-format off
      .add("issuingDetail", ::utility::toObject<IssuingData>(decodedData->issuingDetail, [](auto const &issuingDetail, auto &builder)
        { builder
            .add("securityProviderNum", issuingDetail.securityProviderNum)
            .add("securityProvider", issuingDetail.securityProviderIA5)
            .add("issuerNum", issuingDetail.issuerNum)
            .add("issuer", issuingDetail.issuerIA5)
            .add("issuingDate", u_flex::utility::daysAndYearToIsoDate(issuingDetail.issuingYear, issuingDetail.issuingDay))
            .add("issuingTime", u_flex::utility::minutesToIsoTime(issuingDetail.issuingTime))
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
            .add("pointOfSale", ::utility::toObject<GeoCoordinateType>(issuingDetail.pointOfSale, [](auto const& coordinate, auto &builder)
              { builder
                  .add("geoUnit", ::utility::toString(coordinate.geoUnit))
                  .add("coordinateSystem", ::utility::toString(coordinate.coordinateSystem))
                  .add("hemisphereLongitude", ::utility::toString(coordinate.hemisphereLongitude))
                  .add("hemisphereLatitude", ::utility::toString(coordinate.hemisphereLatitude))
                  .add("longitude", coordinate.longitude)
                  .add("latitude", coordinate.latitude)
                  .add("accuracy", ::utility::toString(coordinate.accuracy)); })); }))
      .add("travelerDetail", ::utility::toObject<TravelerData>(decodedData->travelerDetail, [](auto const &travelerDetail, auto &builder)
        { builder
            .add("preferredLanguage", travelerDetail.preferredLanguage)
            .add("groupName", travelerDetail.groupName)
            .add("traveler", ::utility::toArray<TravelerType>(travelerDetail.traveler, [](auto const &traveler, auto &builder)
              { builder
                  .add("firstName", traveler.firstName)
                  .add("secondName", traveler.secondName)
                  .add("lastName", traveler.lastName)
                  .add("idCard", traveler.idCard)
                  .add("passportId", traveler.passportId)
                  .add("title", traveler.title)
                  .add("gender", ::utility::toString(traveler.gender))
                  .add("customerId", traveler.customerIdIA5)
                  .add("customerIdNum", traveler.customerIdNum)
                  .add("dateOfBirth", u_flex::utility::daysAndYearToIsoDate(traveler.yearOfBirth, traveler.dayOfBirthInMonth))
                  .add("ticketHolder", traveler.ticketHolder)
                  .add("passengerType", ::utility::toString(traveler.passengerType))
                  .add("passengerWithReducedMobility", traveler.passengerWithReducedMobility)
                  .add("countryOfResidence", traveler.countryOfResidence)
                  .add("countryOfPassport", traveler.countryOfPassport)
                  .add("countryOfIdCard", traveler.countryOfIdCard)
                  .add("status", ::utility::toArray<CustomerStatusType>(traveler.status, [](auto const& status, auto &builder)
                  { builder
                        .add("statusProvider", status.statusProviderIA5)                        
                        .add("customerStatus", status.customerStatus)
                        .add("customerStatusDescr", status.customerStatusDescr); })); })); }))
      .add("transportDocuments", ::utility::toArray<DocumentData>(decodedData->transportDocument, [&](auto const &documentData, auto &builder)
        {
          switch (documentData.ticket.present)
          {
          case DocumentData__ticket_PR_openTicket:
          {
            auto const openTicket = documentData.ticket.choice.openTicket;
            builder
              .add("token", ::utility::toObject<TokenType>(documentData.token, [](auto const& token, auto &builder)
                { builder
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
                .add("validRegion", ::utility::toArray<RegionalValidityType>(openTicket.validRegion, [](auto const& region, auto &builder)
                  { /* TODO implement me */ }))
                .add("returnDescription", ::utility::toObject<ReturnRouteDescriptionType>(openTicket.returnDescription, [](auto const& description, auto &builder)
                  { builder
                      .add("fromStationNum", description.fromStationNum)
                      .add("fromStationIA5", description.fromStationIA5)
                      .add("toStationNum", description.toStationNum)
                      .add("toStationIA5", description.toStationIA5)
                      .add("fromStationNameUTF8", description.fromStationNameUTF8)
                      .add("toStationNameUTF8", description.toStationNameUTF8)
                      .add("validReturnRegionDesc", description.validReturnRegionDesc)
                      .add("validRegion", ::utility::toArray<RegionalValidityType>(description.validReturnRegion, [](auto const& region, auto &builder)
                        { /* TODO implement me */ })); }))
                .add("validFromDate", u_flex::utility::daysAndYearToIsoDate(issuingDate.first, issuingDate.second + openTicket.validFromDay))
                .add("validFromTime", u_flex::utility::minutesToIsoTime(openTicket.validFromTime))
                .add("validFromUTCOffset", u_flex::utility::quaterHoursToIsoZone(openTicket.validFromUTCOffset))
                .add("validUntilDate", u_flex::utility::daysAndYearToIsoDate(issuingDate.first, issuingDate.second + openTicket.validFromDay + openTicket.validUntilDay))
                .add("validUntilTime", u_flex::utility::minutesToIsoTime(openTicket.validUntilTime))
                .add("validUntilUTCOffset", u_flex::utility::quaterHoursToIsoZone(openTicket.validUntilUTCOffset))
                .add("activatedDay", ::utility::toArray(openTicket.activatedDay))
                .add("classCode", ::utility::toString(openTicket.classCode))
                .add("serviceLevel", openTicket.serviceLevel)
                .add("carrierNum", ::utility::toArray(openTicket.carrierNum))
                .add("carrier", ::utility::toArray(openTicket.carrierIA5))
                .add("includedServiceBrands", ::utility::toArray(openTicket.includedServiceBrands))
                .add("excludedServiceBrands", ::utility::toArray(openTicket.excludedServiceBrands))
                .add("tariffs", ::utility::toArray<TariffType>(openTicket.tariffs, [](auto const &tariff, auto &builder)
                  { builder
                      .add("numberOfPassengers", tariff.numberOfPassengers)
                      .add("passengerType", ::utility::toString(tariff.passengerType)) // adult: 0, senior: 1, child: 2, youth: 3, dog: 4, bicycle: 5, freeAddonPassenger: 6, freeAddonChild: 7
                      .add("ageBelow", tariff.ageBelow)
                      .add("ageAbove", tariff.ageAbove)
                      .add("travelerid", ::utility::toArray(tariff.travelerid))
                      .add("restrictedToCountryOfResidence", tariff.restrictedToCountryOfResidence)
                      .add("restrictedToRouteSection", ::utility::toObject<RouteSectionType>(tariff.restrictedToRouteSection, [](auto const& route, auto &builder)
                        { builder
                            .add("stationCodeTable", ::utility::toString(route.stationCodeTable))
                            .add("fromStationNum", route.fromStationNum)
                            .add("fromStation", route.fromStationIA5)
                            .add("toStationNum", route.toStationNum)
                            .add("toStation", route.toStationIA5)
                            .add("fromStationName", route.fromStationNameUTF8)
                            .add("toStationName", route.toStationNameUTF8); }))
                      .add("seriesDataDetails", ::utility::toObject<SeriesDetailType>(tariff.seriesDataDetails, [](auto const& series, auto &builder)
                        { builder
                            .add("supplyingCarrier", series.supplyingCarrier)
                            .add("offerIdentification", series.offerIdentification)
                            .add("series", series.series); }))
                      .add("tariffIdNum", tariff.tariffIdNum)
                      .add("tariffId", tariff.tariffIdIA5)
                      .add("tariffDesc", tariff.tariffDesc)
                      .add("reductionCard", ::utility::toArray<CardReferenceType>(tariff.reductionCard, [](auto const& card, auto &builder)
                        { builder
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
                .add("vatDetail", ::utility::toArray<VatDetailType>(openTicket.vatDetail, [](auto const &detail, auto &builder)
                  { builder
                      .add("country", detail.country)
                      .add("percentage", detail.percentage)
                      .add("amount", detail.amount)
                      .add("vatId", detail.vatId); }))
                .add("infoText", openTicket.infoText)
                .add("includedAddOns", ::utility::toArray<IncludedOpenTicketType>(openTicket.includedAddOns, [](auto const addOns, auto &builder)
                  {  /* TODO implement me */ }))
                .add("luggage", ::utility::toObject<LuggageRestrictionType>(openTicket.luggage, [](auto const& luggage, auto &builder)
                  { builder
                      .add("maxHandLuggagePieces", luggage.maxHandLuggagePieces)
                      .add("maxNonHandLuggagePieces", luggage.maxNonHandLuggagePieces)
                      .add("registeredLuggage", ::utility::toArray<RegisteredLuggageType>(luggage.registeredLuggage, [](auto const& registered, auto &builder)
                        { builder
                            .add("maxWeight", registered.maxWeight)
                            .add("maxSize", registered.maxSize); })); }));
          } break;
          case DocumentData__ticket_PR_customerCard: 
          {
            auto const customerCard = documentData.ticket.choice.customerCard;
            builder
              .add("customerCard", ::utility::JsonBuilder::object()
                .add("customer", ::utility::toObject<TravelerType>(customerCard.customer, [](auto const &customer, auto &builder)
                  { builder
                      .add("firstName", customer.firstName)
                      .add("secondName", customer.secondName)
                      .add("lastName", customer.lastName)
                      .add("idCard", customer.idCard)
                      .add("passportId", customer.passportId)
                      .add("title", customer.title)
                      .add("gender", ::utility::toString(customer.gender))
                      .add("customerId", customer.customerIdIA5)
                      .add("customerIdNum", customer.customerIdNum)
                      .add("dateOfBirth", u_flex::utility::daysAndYearToIsoDate(customer.yearOfBirth, customer.dayOfBirthInMonth))
                      .add("ticketHolder", customer.ticketHolder)
                      .add("passengerType", ::utility::toString(customer.passengerType))
                      .add("passengerWithReducedMobility", customer.passengerWithReducedMobility)
                      .add("countryOfResidence", customer.countryOfResidence)
                      .add("countryOfPassport", customer.countryOfPassport)
                      .add("countryOfIdCard", customer.countryOfIdCard)
                      .add("status", ::utility::toArray<CustomerStatusType>(customer.status, [](auto const& status, auto &builder)
                        { builder
                            .add("statusProvider", status.statusProviderIA5)
                            .add("customerStatus", status.customerStatus)
                            .add("customerStatusDescr", status.customerStatusDescr); })); }))
                .add("cardId", customerCard.cardIdIA5)
                .add("cardIdNum", customerCard.cardIdNum)
                .add("validFromDate", u_flex::utility::daysAndYearToIsoDate(customerCard.validFromYear, customerCard.validFromDay == nullptr ? 1l : *customerCard.validFromDay))
                .add("validUntilDate", u_flex::utility::daysAndYearToIsoDate(customerCard.validFromYear + customerCard.validUntilYear, customerCard.validUntilDay == nullptr ? 1l : *customerCard.validUntilDay))
                .add("classCode", ::utility::toString(customerCard.classCode))
                .add("cardType", customerCard.cardType)
                .add("cardTypeDescription", customerCard.cardTypeDescr)
                .add("customerStatus", customerCard.customerStatus)
                .add("customerStatusDescription", customerCard.customerStatusDescr)
                //.add("includedServices", ::utility::toArray(customerCard.includedServices))
              );
          } break;
          default: 
          {
            LOG_WARN(logger) << "Unimplemented transport document data type: " << documentData.ticket.present;
          } break;
          }
      })).build();

    return std::make_optional(::utility::JsonBuilder(std::move(recordJson))); // clang-format on
  }
}
