#pragma once

#include "SignatureChecker.h"

#include "lib/infrastructure/include/ContextFwd.h"

#include "Record.h"

#include <string>
#include <map>
#include <vector>
#include <optional>
#include <memory>
#include <cstdint>

namespace uic918::api
{
  class Interpreter
  {
  public:
    virtual ~Interpreter() = default;

    /* Decodes all known records/fields from given uic918 buffer into json structure.
     */
    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, std::string origin, int indent = -1) const = 0;

    static std::unique_ptr<Interpreter> create(infrastructure::Context &context, SignatureChecker const &signatureChecker);

    static std::unique_ptr<Interpreter> create(infrastructure::Context &context);
  };
}
