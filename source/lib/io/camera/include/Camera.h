#pragma once

#include "lib/io/api/include/InputElement.h"

namespace io::camera
{
  api::InputElement readCamera(unsigned int device = 0);

  void releaseCamera();
}
