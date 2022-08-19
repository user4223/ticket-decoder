#pragma once

#include "../include/Field.h"

#include <string>
#include <vector>
#include <optional>
#include <map>

namespace uic918
{
  class Record;
}

namespace uic918::detail
{
  struct Context
  {
    using BytesType = std::vector<std::uint8_t>;

    virtual ~Context() = default;

    virtual BytesType::const_iterator &getPosition() = 0;

    virtual bool isEmpty() = 0;

    virtual std::size_t getRemainingSize() = 0;

    /* TODO Remove all Field methods
     */
    virtual std::map<std::string, Field> const &getFields() = 0;

    virtual std::optional<Field> getField(std::string key) = 0;

    virtual Context &setField(std::string key, Field &&field) = 0;

    virtual Context &addField(std::string key, std::string value) = 0;

    virtual Context &addField(std::string key, std::string value, std::string description) = 0;

    virtual Context &addField(std::string key, std::string value, std::optional<std::string> description) = 0;
    /* TODO Remove all Field methods
     */
    virtual std::optional<std::string> getJson(unsigned int indent = 0) = 0;

    virtual Context &addRecord(Record &&record) = 0;

    virtual std::optional<Record> tryGetRecord(std::string recordKey) = 0;

    virtual Record getRecord(std::string recordKey) = 0;

    virtual std::map<std::string, Record> const &getRecords() = 0;
  };
}
