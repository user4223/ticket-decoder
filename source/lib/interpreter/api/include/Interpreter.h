#pragma once

#include "lib/infrastructure/include/ContextFwd.h"

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <cstdint>

namespace interpreter::api
{
  class SignatureVerifier;

  class Interpreter
  {
  public:
    virtual ~Interpreter() = default;

    /* Decodes all known records/fields from given buffer into json structure.
     */
    virtual std::optional<std::string> interpret(std::vector<std::uint8_t> const &input, std::string origin, int indent = -1) const = 0;

    static std::unique_ptr<Interpreter> create(infrastructure::Context &context, SignatureVerifier const &signatureChecker);

    static std::unique_ptr<Interpreter> create(infrastructure::Context &context);
  };
}
