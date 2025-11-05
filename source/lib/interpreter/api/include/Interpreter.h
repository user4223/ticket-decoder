#pragma once

#include "SignatureChecker.h"

#include "lib/infrastructure/include/ContextFwd.h"

#include <string>
#include <map>
#include <vector>
#include <optional>
#include <memory>
#include <cstdint>

/* TODO This should move out of UIC918 and should move into a generic module, since internal parts VDV and SBB are not UIC918
 */
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
