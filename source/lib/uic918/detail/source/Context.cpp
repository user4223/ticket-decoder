
#include "../include/Context.h"

#include "lib/uic918/api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"

namespace uic918::detail
{
  Context::Context(std::vector<std::uint8_t> const &i)
      : input(i),
        position(input.begin()),
        output()
  {
  }

  Context::Context(std::vector<std::uint8_t> const &i, std::map<std::string, Field> &&f)
      : input(i),
        position(input.begin()),
        output(std::move(f))
  {
  }

  std::vector<std::uint8_t>::const_iterator &Context::getPosition()
  {
    return position;
  }

  bool Context::isEmpty()
  {
    return position == input.cend();
  }

  std::size_t Context::getRemainingSize()
  {
    return std::distance(position, input.end());
  }

  std::map<std::string, Field> const &Context::getFields()
  {
    return output;
  }

  std::optional<Field> Context::getField(std::string key)
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

  std::optional<std::string> Context::getJson()
  {
    if (records.empty())
    {
      return std::nullopt;
    }
    using json = nlohmann::json;
    auto result = json::object();
    result["records"] = std::reduce(records.begin(), records.end(), json::object(),
                                    [](auto &&result, auto const &record)
                                    {
                                      result[record.first] = json::parse(record.second.getJson());
                                      return std::move(result);
                                    });
    return std::make_optional(std::move(result.dump()));
  }

  Context &Context::addRecord(api::Record &&record)
  {
    auto const id = record.getId();
    records.insert(std::make_pair(id, std::move(record)));
    return *this;
  }

  std::optional<api::Record> Context::tryGetRecord(std::string recordKey)
  {
    auto const record = records.find(recordKey);
    return record == records.end() ? std::nullopt : std::make_optional(record->second);
  }

  api::Record Context::getRecord(std::string recordKey)
  {
    auto const record = tryGetRecord(recordKey);
    if (!record)
    {
      throw std::runtime_error("Record not found: " + recordKey);
    }
    return *record;
  }

  std::map<std::string, api::Record> const &Context::getRecords()
  {
    return records;
  }

}
