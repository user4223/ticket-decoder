// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVInterpreter.h"
#include "../include/LDIFFileCertificateProvider.h"
#include "../include/BotanMessageDecoder.h"
#include "../include/VDVUtility.h"
#include "../include/Envelop.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

#include "lib/utility/include/Base64.h"

#include "lib/infrastructure/include/Logging.h"

#include <array>

namespace interpreter::detail::vdv
{

  static std::vector<std::uint8_t> const typeId = {0x9E, 0x81, 0x80};

  VDVInterpreter::TypeIdType VDVInterpreter::getTypeId()
  {
    return typeId;
  }

  VDVInterpreter::VDVInterpreter(infrastructure::LoggerFactory &lf)
      : logger(CREATE_LOGGER(lf)),
        certificateProvider(std::make_unique<LDIFFileCertificateProvider>(lf)),
        messageDecoder(std::make_unique<BotanMessageDecoder>(lf, *certificateProvider))
  {
  }

  struct RecordDescriptor
  {
    std::string const name;
    std::optional<std::size_t> const expectedSize;
    std::function<void(std::span<std::uint8_t const> const &data, Envelop &)> const collector;
    std::function<void(std::string const &name, Envelop const &, utility::JsonBuilder &)> const consumer;
  };

  class RecordBuilder
  {
    static std::map<TagType, RecordDescriptor> const tagDescriptorMap;

    MessageDecoder &messageDecoder;
    Envelop envelop;

  public:
    RecordBuilder(MessageDecoder &md) : messageDecoder(md) {}

    bool add(TagType const &tag, std::span<std::uint8_t const> const &data)
    {
      auto const entry = tagDescriptorMap.find(tag);
      if (entry == tagDescriptorMap.end())
      {
        return false;
      }

      auto const &descriptor = entry->second;
      if (descriptor.expectedSize)
      {
        if (data.size() != *descriptor.expectedSize)
        {
          throw std::runtime_error(std::string("Expecting exactly ") + std::to_string(*descriptor.expectedSize) + " payload bytes for tag 0x" + common::bytesToHexString(tag) + " (" + descriptor.name + "), got: " + std::to_string(data.size()));
        }
      }
      descriptor.collector(data, envelop);
      return true;
    }

    common::Record build() const
    {
      auto const message = messageDecoder.decode(envelop);

      auto jsonBuilder = utility::JsonBuilder::object();

      std::for_each(std::begin(tagDescriptorMap), std::end(tagDescriptorMap), [&](auto const &entry)
                    {
                      auto const &descriptor = entry.second;
                      descriptor.consumer(descriptor.name, envelop, jsonBuilder); });

      return common::Record(envelop.kennung, envelop.version, std::move(jsonBuilder));
    }
  };

  std::map<TagType, RecordDescriptor> const RecordBuilder::tagDescriptorMap = {
      {TagType{0x9e, 0x00}, RecordDescriptor{"signature", 128,
                                             [](auto const &data, auto &envelop)
                                             { envelop.signature = data; },
                                             [](auto const &name, auto const &envelop, auto &jsonBuilder)
                                             { jsonBuilder.add(name, utility::base64::encode(envelop.signature)); }}},
      {TagType{0x9a, 0x00}, RecordDescriptor{"identifier", 5,
                                             [](auto const &data, auto &envelop)
                                             { 
                                               envelop.kennung = common::bytesToAlphanumeric(data.subspan(0, 3));
                                               envelop.version = common::bytesToHexString(data.subspan(3, 2)); },
                                             [](auto const &name, auto const &envelop, auto &jsonBuilder)
                                             { jsonBuilder
                                                   .add("kennung", envelop.kennung)
                                                   .add("version", envelop.version); }}},
      {TagType{0x7f, 0x21}, RecordDescriptor{"certificate", 200, [](auto const &data, auto &envelop)
                                             { envelop.certificate = data; },
                                             [](auto const &name, auto const &envelop, auto &jsonBuilder)
                                             { jsonBuilder.add(name, utility::base64::encode(envelop.certificate)); }}},
      {TagType{0x42, 0x00}, RecordDescriptor{"authority", 8, [](auto const &data, auto &envelop)
                                             { envelop.authority = common::bytesToHexString(data); },
                                             [](auto const &name, auto const &envelop, auto &jsonBuilder)
                                             { jsonBuilder.add(name, envelop.authority); }}}};

  common::Context VDVInterpreter::interpret(common::Context &&context)
  {
    // Documentation (not fully matching anymore):
    //  - https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf
    // Quite old reference impl for encoding:
    //  - https://sourceforge.net/projects/dbuic2vdvbc/
    // Some more up-to-date hints:
    //  - https://magicalcodewit.ch/38c3-slides/#/32
    //  - https://github.com/akorb/deutschlandticket_parser/blob/main/main.py
    //  - https://github.com/RWTH-i5-IDSG/ticketserver/blob/master/barti-check/src/main/java/de/rwth/idsg/barti/check/Decode.java

    auto recordBuilder = RecordBuilder(*messageDecoder);
    while (context.getRemainingSize() > 0)
    {
      auto const tag = getTag(context);
      auto const length = getLength(context);

      if (!recordBuilder.add(tag, context.consumeBytes(length)))
      {
        LOG_WARN(logger) << "Unknown record found: tag: 0x" << common::bytesToHexString(tag) << ", length: " << length;
      }
    }

    context.addRecord(recordBuilder.build());
    return std::move(context);
  }
}
