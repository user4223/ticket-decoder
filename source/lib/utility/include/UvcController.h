#pragma once

#include <memory>

namespace utility
{
  class UvcController
  {
    struct Internal;

    std::shared_ptr<Internal> internal;

  public:
    UvcController();
  };
}
