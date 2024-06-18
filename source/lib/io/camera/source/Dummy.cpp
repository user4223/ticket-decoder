
#include "../include/Camera.h"

#include <stdexcept>

namespace io::camera
{

  api::InputElement readCamera(unsigned int device)
  {
    throw std::runtime_error("Camera support disabled at compile-time");
  }

  void releaseCamera()
  {
  }
}
