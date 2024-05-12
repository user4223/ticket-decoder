#pragma once

#include "SignatureChecker.h"

#include "lib/utility/include/LoggingFwd.h"

#include "Record.h"

#include <string>
#include <map>
#include <vector>
#include <optional>

namespace uic918::api
{
  class Interpreter
  {
  public:
    static std::unique_ptr<Interpreter> create(
        ::utility::LoggerFactory &loggerFactory,
        SignatureChecker const &signatureChecker);

    static std::unique_ptr<Interpreter> create(
        ::utility::LoggerFactory &loggerFactory);

    virtual ~Interpreter() = default;

    /* Decodes all known records/fields from given uic918 buffer into json structure.
     */
    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, std::string origin, int indent = -1) const = 0;

    /* Decodes all known records from given uic918 buffer into per record json structure.
     */
    virtual std::map<std::string, Record> interpretRecords(std::vector<std::uint8_t> const &input, std::string origin) const = 0;
  };
}
