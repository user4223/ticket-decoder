#pragma once

#include "Record.h"

#include <string>
#include <map>
#include <vector>
#include <optional>

namespace uic918
{
  class Interpreter
  {
  public:
    /* Decodes all known records/fields from given uic918 buffer into json structure.
     */
    static std::optional<std::string> interpret(std::vector<std::uint8_t> const &input);

    /* Decodes all known records/fields from given uic918 buffer into pretty formatted - human readable - json structure.
     */
    static std::optional<std::string> interpretPretty(std::vector<std::uint8_t> const &input, unsigned int indent = 3);

    /* Decodes all known records from given uic918 buffer into per record json structure.
     */
    static std::map<std::string, Record> interpretRecords(std::vector<std::uint8_t> const &input);
  };
}
