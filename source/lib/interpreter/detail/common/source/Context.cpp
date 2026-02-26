// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Context.h"
#include "../include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Base64.h"

#include <algorithm>

namespace interpreter::detail::common
{
  Context::Context(std::vector<std::uint8_t> const &input, std::string origin)
      : raw(std::nullopt),
        data(input.data(), input.size()),
        begin(std::begin(data)),
        position(begin),
        end(std::end(data)),
        output(),
        records()
  {
    addField("origin", origin);
  }

  Context::Context(std::vector<std::uint8_t> const &input, Context &&otherContext)
      : raw(std::nullopt),
        data(input.data(), input.size()),
        begin(std::begin(data)),
        position(begin),
        end(std::end(data)),
        output(std::move(otherContext.output)),
        records(std::move(otherContext.records))
  {
  }

  Context::Context(std::span<std::uint8_t const> input)
      : raw(std::nullopt),
        data(std::move(input)),
        begin(std::begin(data)),
        position(begin),
        end(std::end(data)),
        output(),
        records()
  {
  }

  Context::Context(std::vector<std::uint8_t> &&input)
      : raw(std::make_optional(std::move(input))),
        data(std::begin(*raw), std::end(*raw)),
        begin(std::begin(data)),
        position(begin),
        end(std::end(data)),
        output(),
        records()
  {
  }

  Context::IteratorType Context::getPosition() const
  {
    return position;
  }

  std::uint8_t Context::peekByte() const
  {
    ensureRemaining(1);
    return *position;
  }

  std::uint8_t Context::peekByte(std::size_t offset) const
  {
    ensureRemaining(offset + 1);
    return *(position + offset);
  }

  std::span<std::uint8_t const> Context::peekBytes(std::size_t size) const
  {
    ensureRemaining(size);
    return std::span<std::uint8_t const>(position, size);
  }

  std::span<std::uint8_t const> Context::peekBytes(std::size_t offset, std::size_t size) const
  {
    ensureRemaining(offset + size);
    return std::span<std::uint8_t const>(position + offset, size);
  }

  std::uint8_t Context::consumeByte()
  {
    ensureRemaining(1);
    auto value = *position;
    position += 1;
    return value;
  }

  std::span<std::uint8_t const> Context::consumeBytes(std::size_t size)
  {
    ensureRemaining(size);
    auto result = std::span<std::uint8_t const>(position, size);
    position += size;
    return result;
  }

  std::uint8_t Context::consumeByteEnd()
  {
    ensureRemaining(1);
    end -= 1;
    return *end;
  }

  std::span<std::uint8_t const> Context::consumeBytesEnd(std::size_t size)
  {
    ensureRemaining(size);
    end -= size;
    return std::span<std::uint8_t const>(end, size);
  }

  std::span<std::uint8_t const> Context::consumeMaximalBytes(std::size_t size)
  {
    return consumeBytes(std::min(getRemainingSize(), size));
  }

  std::span<std::uint8_t const> Context::consumeRemainingBytes()
  {
    return consumeBytes(getRemainingSize());
  }

  std::vector<std::uint8_t> Context::consumeRemainingBytesCopy()
  {
    auto const data = consumeRemainingBytes();
    return {data.begin(), data.end()};
  }

  std::vector<std::uint8_t> Context::consumeRemainingBytesAppend(std::span<std::uint8_t const> postfix)
  {
    auto data = consumeRemainingBytesCopy();
    data.insert(data.end(), postfix.begin(), postfix.end());
    return data;
  }

  std::size_t Context::ignoreBytes(std::size_t size)
  {
    ensureRemaining(size);
    std::advance(position, size);
    return size;
  }

  bool Context::ignoreBytesIf(std::vector<std::uint8_t> expectedValue)
  {
    if (getRemainingSize() < expectedValue.size())
    {
      return false;
    }

    auto index = std::size_t(0);
    for (auto const value : expectedValue)
    {
      if (value != peekByte(index++))
      {
        return false;
      }
    }

    ignoreBytes(expectedValue.size());
    return true;
  }

  std::size_t Context::ignoreRemainingBytes()
  {
    return ignoreBytes(getRemainingSize());
  }

  std::string Context::getAllBase64Encoded() const
  {
    return utility::base64::encode(&(*begin), getOverallSize());
  }

  bool Context::hasInput() const
  {
    return data.size() > 0;
  }

  bool Context::hasOutput() const
  {
    return output.size() > 0 || records.size() > 0;
  }

  bool Context::isEmpty() const
  {
    return position == end;
  }

  void Context::ensureEmpty() const
  {
    if (!isEmpty())
    {
      throw std::runtime_error(std::string("Expecting fully consumed context, but found remaining bytes: ") + std::to_string(getRemainingSize()));
    }
  }

  void Context::ensureRemaining(std::size_t size) const
  {
    if (getRemainingSize() < size)
    {
      throw std::runtime_error(std::string("Less than expected bytes available, expecting at least: ") + std::to_string(size));
    }
  }

  std::map<std::string, Field> const &Context::getFields() const
  {
    return output;
  }

  std::optional<Field> Context::getField(std::string key) const
  {
    auto const entry = output.find(key);
    return entry == output.end() ? std::optional<Field>{} : entry->second;
  }

  Context &Context::setField(std::string key, Field &&field)
  {
    output.insert_or_assign(key, std::move(field));
    return *this;
  }

  Context &Context::ifField(std::string key, std::function<void(std::string const &)> consumer)
  {
    auto field = getField(key);
    if (field)
    {
      consumer(field->getValue());
    }
    return *this;
  }

  Context &Context::addField(std::string key, std::string value)
  {
    return addField(key, value, std::nullopt);
  }

  Context &Context::addField(std::string key, std::string value, std::string description)
  {
    return addField(key, value, std::make_optional(description));
  }

  Context &Context::addField(std::string key, std::string value, std::optional<std::string> description)
  {
    return setField(key, Field{value, description});
  }

  std::optional<std::string> Context::getJson(int indent)
  {
    if (records.empty())
    {
      return std::nullopt;
    }

    auto builder = ::utility::JsonBuilder::object();
    ifField("raw", [&](auto const &value)
            { builder.add("raw", value); });
    ifField("origin", [&](auto const &value)
            { builder.add("origin", value); });
    ifField("validated", [&](auto const &value)
            { builder.add("validated", value); });
    ifField("companyCode", [&](auto const &value)
            { builder.add("companyCode", value); });
    ifField("signatureKeyId", [&](auto const &value)
            { builder.add("signatureKeyId", value); });

    builder.add("records", std::accumulate(records.begin(), records.end(), ::utility::JsonBuilder::object(),
                                           [](auto &&builder, auto const &record)
                                           {
                                              builder.add(record.first, std::move(record.second.getJson()));
                                              return std::move(builder); })
                               .build());

    return std::make_optional(builder.buildString(indent));
  }

  Context &Context::addRecord(Record &&record)
  {
    auto const id = record.getId();
    records.insert(std::make_pair(id, std::move(record)));
    return *this;
  }

  Record const &Context::getRecord(std::string recordKey) const
  {
    auto const record = records.find(recordKey);
    if (record == records.end())
    {
      throw std::runtime_error("Record not found: " + recordKey);
    }
    return record->second;
  }

  std::map<std::string, Record> const &Context::getRecords() const
  {
    return records;
  }
}
