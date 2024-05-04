
#include "../include/Context.h"

#include "lib/uic918/api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Base64.h"

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
    output[key] = std::move(field);
    return *this;
  }

  Context &Context::addField(std::string key, std::string value)
  {
    return addField(key, value, std::optional<std::string>{});
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
    using json = nlohmann::json;
    auto result = json::object();
    result["raw"] = getField("raw").value_or(Field{""}).value;
    result["origin"] = getField("origin").value_or(Field{""}).value;
    result["validated"] = getField("validated").value_or(Field{"false"}).value;
    result["records"] = std::accumulate(records.begin(), records.end(), json::object(),
                                        [](auto &&result, auto const &record)
                                        {
                                          result[record.first] = json::parse(record.second.getJson());
                                          return std::move(result);
                                        });
    return std::make_optional(std::move(result.dump(indent)));
  }

  Context &Context::addRecord(api::Record &&record)
  {
    auto const id = record.getId();
    records.insert(std::make_pair(id, std::move(record)));
    return *this;
  }

  std::optional<api::Record> Context::tryGetRecord(std::string recordKey) const
  {
    auto const record = records.find(recordKey);
    return record == records.end() ? std::nullopt : std::make_optional(record->second);
  }

  api::Record Context::getRecord(std::string recordKey) const
  {
    auto const record = tryGetRecord(recordKey);
    if (!record)
    {
      throw std::runtime_error("Record not found: " + recordKey);
    }
    return *record;
  }

  std::map<std::string, api::Record> const &Context::getRecords() const
  {
    return records;
  }

}
