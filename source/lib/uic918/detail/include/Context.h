#pragma once

#include "lib/uic918/api/include/Record.h"

#include "../include/Field.h"

#include <string>
#include <vector>
#include <optional>
#include <map>

namespace uic918::detail
{
  struct Context
  {
    std::vector<std::uint8_t> const &input;
    std::vector<std::uint8_t>::const_iterator position;
    std::map<std::string, Field> output;
    std::map<std::string, api::Record> records;

    Context(std::vector<std::uint8_t> const &i);

    Context(std::vector<std::uint8_t> const &i, std::map<std::string, Field> &&f);

    std::vector<std::uint8_t>::const_iterator &getPosition();

    bool isEmpty();

    std::size_t getRemainingSize();

    // Fields

    std::map<std::string, Field> const &getFields();

    std::optional<Field> getField(std::string key);

    Context &setField(std::string key, Field &&field);

    Context &addField(std::string key, std::string value);

    Context &addField(std::string key, std::string value, std::string description);

    Context &addField(std::string key, std::string value, std::optional<std::string> description);

    // Json

    std::optional<std::string> getJson(int indent = -1);

    // Records

    Context &addRecord(api::Record &&record);

    std::optional<api::Record> tryGetRecord(std::string recordKey);

    api::Record getRecord(std::string recordKey);

    std::map<std::string, api::Record> const &getRecords();
  };
}
