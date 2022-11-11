#pragma once

#include "../include/Field.h"

#include <string>
#include <vector>
#include <optional>
#include <map>

namespace uic918::api
{
  class Record;
}

namespace uic918::detail
{
  struct Context
  {
    virtual ~Context() = default;

    virtual std::vector<std::uint8_t>::const_iterator &getPosition() = 0;

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
    virtual std::optional<std::string> getJson() = 0;

    virtual Context &addRecord(api::Record &&record) = 0;

    virtual std::optional<api::Record> tryGetRecord(std::string recordKey) = 0;

    virtual api::Record getRecord(std::string recordKey) = 0;

    virtual std::map<std::string, api::Record> const &getRecords() = 0;
  };
}
