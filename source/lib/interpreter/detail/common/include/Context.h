#pragma once

#include "Record.h"
#include "Field.h"

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <cstdint>
#include <functional>

namespace interpreter::detail::common
{
  struct Context
  {
    std::size_t inputSize;
    std::vector<std::uint8_t>::const_iterator begin;
    std::vector<std::uint8_t>::const_iterator position;
    std::vector<std::uint8_t>::const_iterator end;
    std::map<std::string, Field> output;
    std::map<std::string, Record> records;

    Context(std::vector<std::uint8_t> const &input, std::string origin);
    Context(std::vector<std::uint8_t> const &input, std::map<std::string, Field> &&f);

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

    Context &ifField(std::string key, std::function<void(std::string const &)> consumer);

    Context &setField(std::string key, Field &&field);

    Context &addField(std::string key, std::string value);

    Context &addField(std::string key, std::string value, std::string description);

    Context &addField(std::string key, std::string value, std::optional<std::string> description);

    // Json

    std::optional<std::string> getJson(int indent = -1);

    // Records

    Context &addRecord(Record &&record);

    Record const &getRecord(std::string recordKey) const;

    std::map<std::string, Record> const &getRecords() const;
  };
}
