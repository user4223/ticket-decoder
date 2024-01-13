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
    std::size_t inputSize;
    std::vector<std::uint8_t>::const_iterator begin;
    std::vector<std::uint8_t>::const_iterator position;
    std::vector<std::uint8_t>::const_iterator end;
    std::map<std::string, Field> output;
    std::map<std::string, api::Record> records;
    std::string origin;

    Context(std::vector<std::uint8_t> const &input, std::string origin);
    Context(std::vector<std::uint8_t> const &input, std::string origin, std::map<std::string, Field> &&f);

    Context(Context const &) = delete;
    Context &operator=(Context const &) = delete;

    Context(Context &&) = default;
    Context &operator=(Context &&) = default;

    std::vector<std::uint8_t>::const_iterator &getPosition();

    bool hasInput() const;

    bool hasOutput() const;

    bool isEmpty() const;

    std::size_t getRemainingSize() const;

    std::size_t getConsumedSize() const;

    std::string getBase64Encoded() const;

    // Fields

    std::map<std::string, Field> const &getFields() const;

    std::optional<Field> getField(std::string key) const;

    Context &setField(std::string key, Field &&field);

    Context &addField(std::string key, std::string value);

    Context &addField(std::string key, std::string value, std::string description);

    Context &addField(std::string key, std::string value, std::optional<std::string> description);

    // Json

    std::optional<std::string> getJson(int indent = -1);

    // Records

    Context &addRecord(api::Record &&record);

    std::optional<api::Record> tryGetRecord(std::string recordKey) const;

    api::Record getRecord(std::string recordKey) const;

    std::map<std::string, api::Record> const &getRecords() const;
  };
}
