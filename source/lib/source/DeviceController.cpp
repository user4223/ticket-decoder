
#include "../include/DeviceController.h"

#include "libuvc/libuvc.h"

#include <stdexcept>
#include <string>

struct DeviceController::Internal
{
  uvc_context_t *context = nullptr;
  uvc_device_t *device = nullptr;
  uvc_device_handle_t *handle = nullptr;

  Internal()
  {
    {
      auto const result = uvc_init(&context, NULL);
      if (result < 0)
      {
        throw std::runtime_error("uvc_init failed with: " + std::to_string(result));
      }
    }
    {
      auto const result = uvc_find_device(context, &device, 0, 0, NULL);
      if (result < 0)
      {
        throw std::runtime_error("uvc_find_device failed with: " + std::to_string(result));
      }
    }
    // Not working due to:
    // https://github.com/libusb/libusb/issues/1014
    // https://github.com/libusb/libusb/wiki/FAQ#How_can_I_run_libusb_applications_under_Mac_OS_X_if_there_is_already_a_kernel_extension_installed_for_the_device_and_claim_exclusive_access
    {
      auto const result = uvc_open(device, &handle);
      if (result < 0)
      {
        throw std::runtime_error("uvc_open failed with: " + std::to_string(result));
      }
    }
    uvc_print_diag(handle, stdout);
  }

  ~Internal()
  {
    if (handle != nullptr)
    {
      uvc_close(handle);
    }
    if (device != nullptr)
    {
      uvc_unref_device(device);
    }
    if (context != nullptr)
    {
      uvc_exit(context);
    }
  }
};

DeviceController::DeviceController() : internal(std::make_shared<Internal>())
{
}