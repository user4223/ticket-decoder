#pragma once

#include <memory>

class DeviceController
{
  struct Internal;

  std::shared_ptr<Internal> internal;

public:
  DeviceController();
};
