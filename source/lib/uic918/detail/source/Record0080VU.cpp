
#include "../include/Record0080VU.h"
#include "../include/Utility.h"

#include "../../api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

#include "sstream"

namespace uic918::detail
{
  ::utility::JsonBuilder toArray(unsigned int byteSize, std::function<std::tuple<::utility::JsonBuilder, std::size_t>()> producer)
  {
    auto builder = ::utility::JsonBuilder::array();
    for (auto consumedSize = 0; consumedSize < byteSize;)
    {
      auto [json, size] = producer();
      consumedSize += size;
      builder.add(std::move(json));
    }
    return builder;
  }

  Record0080VU::Record0080VU(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("0080VU", {"01"});
  }

  Context Record0080VU::interpret(Context &&context)
  {
    auto recordJson = ::utility::JsonBuilder::object() // clang-format off
      .add("terminalNummer", std::to_string(utility::getNumeric16(context.getPosition())))
      .add("samNummer", std::to_string(utility::getNumeric24(context.getPosition())))
      .add("anzahlPersonen", std::to_string(utility::getNumeric8(context.getPosition())))
      .add("efs", ::utility::toArray(utility::getNumeric8(context.getPosition()), 
        [&](){
          // TODO Unsure if numeric is the proper interpretation of berechtigungsNummer
          // kvp -> Kundenvertragspartner
          // pv -> Produktverantwortlicher
          return ::utility::JsonBuilder::object()        
            .add("berechtigungsNummer", std::to_string(utility::getNumeric32(context.getPosition())))
            .add("kvpOrganisationsId", std::to_string(utility::getNumeric16(context.getPosition())))
            .add("pvProduktnummer", std::to_string(utility::getNumeric16(context.getPosition())))
            .add("pvOrganisationsId", std::to_string(utility::getNumeric16(context.getPosition())))
            .add("gueltigAb", utility::getDateTimeCompact(context.getPosition()))
            .add("gueltigBis", utility::getDateTimeCompact(context.getPosition()))
            .add("preis", utility::getNumeric24(context.getPosition()))
            .add("samSequenznummer", std::to_string(utility::getNumeric32(context.getPosition())))
            .add("flaechenelemente", toArray(utility::getNumeric8(context.getPosition()), 
              [&](){
                auto tagStream = std::ostringstream();
                auto const tagValue = int(utility::getNumeric8(context.getPosition()));
                tagStream << std::hex << std::noshowbase << tagValue;
                auto const tag = tagStream.str();
                auto const elementLength = utility::getNumeric8(context.getPosition());
                auto const typ = std::to_string(utility::getNumeric8(context.getPosition()));
                auto const organisationsId = std::to_string(utility::getNumeric16(context.getPosition()));
                auto const flaechenIdLength = elementLength - 3;
                if (flaechenIdLength != 2 && flaechenIdLength != 3)
                {
                  throw std::runtime_error(std::string("Unexpected FlaechenelementId length: ") + std::to_string(flaechenIdLength)); 
                }
                auto const flaechenId = std::to_string(flaechenIdLength == 2
                  ? utility::getNumeric16(context.getPosition())
                  : utility::getNumeric24(context.getPosition()));

                auto element = ::utility::JsonBuilder::object()
                  .add("tag", tag)
                  .add("typ", typ)
                  .add("kvpOrganisationsId", organisationsId)
                  .add("flaechenId", flaechenId);

                return std::make_tuple(std::move(element), elementLength + 2); })); })); // clang-format on

    context.addRecord(api::Record(header.recordId, header.recordVersion, recordJson.build()));
    return std::move(context);
  }
}
