#pragma once

#include <memory>

namespace device 
{
  class UvcController
  {
    struct Internal;

    std::shared_ptr<Internal> internal;

  public:
    UvcController();
  };
}
