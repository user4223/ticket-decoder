
#include "../include/Context.h"

#include "lib/uic918/api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Base64.h"

#include <algorithm>

namespace uic918::detail
{
  Context::Context(std::vector<std::uint8_t> const &input, std::string origin)
      : inputSize(input.size()),
        begin(input.cbegin()),
        position(begin),
        end(input.cend())
  {
    addField("origin", origin);
  }

  Context::Context(std::vector<std::uint8_t> const &input, std::map<std::string, Field> &&fields)
      : inputSize(input.size()),
        begin(input.cbegin()),
        position(begin),
        end(input.cend()),
        output(std::move(fields))
  {
  }

  std::vector<std::uint8_t>::const_iterator &Context::getPosition()
  {
    return position;
  }

  bool Context::hasInput() const
  {
    return inputSize > 0;
  }

  bool Context::hasOutput() const
  {
    return output.size() > 0 || records.size() > 0;
  }

  bool Context::isEmpty() const
  {
    return position == end;
  }

  std::size_t Context::getRemainingSize() const
  {
    return std::distance(position, end);
  }

  std::size_t Context::getConsumedSize() const
  {
    return std::distance(begin, position);
  }

  std::string Context::getBase64Encoded() const
  {
    return utility::base64::encode(&(*begin), std::distance(begin, end));
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

  Context &Context::addRecord(api::Record &&record)
  {
    auto const id = record.getId();
    records.insert(std::make_pair(id, std::move(record)));
    return *this;
  }

  api::Record const &Context::getRecord(std::string recordKey) const
  {
    auto const record = records.find(recordKey);
    if (record == records.end())
    {
      throw std::runtime_error("Record not found: " + recordKey);
    }
    return record->second;
  }

  std::map<std::string, api::Record> const &Context::getRecords() const
  {
    return records;
  }
}
