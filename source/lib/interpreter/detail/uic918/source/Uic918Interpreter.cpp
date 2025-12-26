// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Uic918Interpreter.h"

#include "../include/RecordHeader.h"
#include "../include/RecordU_HEAD.h"
#include "../include/RecordU_TLAY.h"
#include "../include/Record0080BL.h"
#include "../include/Record0080VU.h"
#include "../include/RecordU_FLEX.h"
#include "../include/Record118199.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/Deflator.h"
#include "lib/interpreter/detail/common/include/Field.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/Logging.h"

#include <stdexcept>
#include <functional>
#include <map>
#include <optional>

namespace interpreter::detail::uic
{
  static const std::map<std::string, std::function<std::unique_ptr<common::Interpreter>(::utility::LoggerFactory &loggerFactory, RecordHeader &&)>> recordInterpreterMap =
      {
          {"U_HEAD", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<RecordU_HEAD>(loggerFactory, std::move(header)); }},
          {"U_TLAY", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<RecordU_TLAY>(loggerFactory, std::move(header)); }},
          {"0080BL", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<Record0080BL>(loggerFactory, std::move(header)); }},
          {"0080VU", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<Record0080VU>(loggerFactory, std::move(header)); }},
          {"U_FLEX", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<RecordU_FLEX>(loggerFactory, std::move(header)); }},
          {"118199", [](auto &loggerFactory, auto &&header)
           { return std::make_unique<Record118199>(loggerFactory, std::move(header)); }}};

  static std::vector<std::uint8_t> const typeId = {'#', 'U', 'T'};

  Uic918Interpreter::TypeIdType Uic918Interpreter::getTypeId()
  {
    return typeId;
  }

  Uic918Interpreter::Uic918Interpreter(::utility::LoggerFactory &lf, api::SignatureVerifier const &sc)
      : loggerFactory(lf), logger(CREATE_LOGGER(lf)), signatureChecker(&sc), messageContext()
  {
  }

  Uic918Interpreter::Uic918Interpreter(::utility::LoggerFactory &lf)
      : loggerFactory(lf), logger(CREATE_LOGGER(lf)), signatureChecker(nullptr), messageContext()
  {
  }

  common::Context Uic918Interpreter::interpret(common::Context &&context)
  {
    auto const tid = context.consumeBytes(typeId.size());
    if (Uic918Interpreter::TypeIdType(tid.begin(), tid.end()) != typeId)
    {
      throw std::runtime_error("Unexpected UIC918 type ID, expecting " + common::bytesToString(typeId) + ", got: " + common::bytesToString(tid));
    }

    if (context.getRemainingSize() < 2)
    {
      LOG_WARN(logger) << "Unable to read message version, less than 2 bytes available";
      return std::move(context);
    }

    auto const messageTypeVersion = common::getAlphanumeric(context, 2);
    auto const version = std::stoi(messageTypeVersion);
    // Might be "OTI" as well
    if (version != 1 && version != 2)
    {
      LOG_WARN(logger) << "Unsupported message version: " << messageTypeVersion;
      return std::move(context);
    }

    context.addField("raw", context.getAllBase64Encoded());
    context.addField("uniqueMessageTypeId", "#UT");
    context.addField("messageTypeVersion", messageTypeVersion);
    auto const ricsCode = common::getAlphanumeric(context, 4);
    context.addField("companyCode", ricsCode);
    auto const keyId = common::getAlphanumeric(context, 5);
    context.addField("signatureKeyId", keyId);

    auto const signatureLength = version == 2 ? 64 : 50;
    auto const signature = context.consumeBytes(signatureLength);
    auto const consumed = context.getConsumedSize();
    auto const messageLengthString = common::getAlphanumeric(context, 4);
    auto const messageLength = std::stoi(messageLengthString);
    context.addField("compressedMessageLength", std::to_string(messageLength));
    if (messageLength < 0 || messageLength > context.getRemainingSize())
    {
      throw std::runtime_error("compressedMessageLength out of range: " + std::to_string(messageLength));
    }
    auto const compressedMessage = context.consumeBytes(messageLength);
    if (!context.isEmpty())
    {
      throw std::runtime_error("Unconsumed bytes in payload");
    }

    if (signatureChecker != nullptr)
    {
      auto const validationResult = signatureChecker->check(ricsCode, keyId, compressedMessage, signature);
      if (validationResult == api::SignatureVerifier::Result::KeyNotFound)
      {
        LOG_DEBUG(logger) << "No certificate available to validate: " << ricsCode << " / " << keyId;
      }
      context.addField("validated", validationResult == api::SignatureVerifier::Result::Successful ? "true" : "false");
    }

    auto const uncompressedMessage = common::deflate(compressedMessage);
    context.addField("uncompressedMessageLength", std::to_string(uncompressedMessage.size()));

    auto messageContext = common::Context(uncompressedMessage, std::move(context.output));
    while (!messageContext.isEmpty())
    {
      LOG_DEBUG(logger) << "Overall remaining bytes: " << messageContext.getRemainingSize();

      auto header = RecordHeader{messageContext};
      auto const entry = recordInterpreterMap.find(header.recordId);
      if (entry != recordInterpreterMap.end())
      {
        auto &factory = entry->second;
        auto interpreter = factory(loggerFactory, std::move(header));
        messageContext = interpreter->interpret(std::move(messageContext));
      }
      else // skip block
      {
        auto const ignored = header.ignoreRemainingRecordBytes(messageContext);
        LOG_WARN(logger) << "Ignoring " << ignored << " bytes containing unknown record: " << header.toString();
      }
    }

    if (!messageContext.isEmpty())
    {
      throw std::runtime_error("Unconsumed bytes in message");
    }

    return std::move(messageContext);
  }
}
