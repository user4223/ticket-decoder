PROJECT(aztec-detector-lib)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("uic918/api/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("uic918/detail/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("uic918/gen" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("barcode/api/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("barcode/detail/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("dip/filtering/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("dip/detection/api/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("dip/detection/detail/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("utility/source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("device/source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES
   "include/*.h"
   "uic918/api/include/*.h"
   "uic918/detail/include/*.h"
   "uic918/gen/*.h"
   "barcode/api/include/*.h"
   "barcode/detail/include/*.h"
   "dip/filtering/include/*.h"
   "dip/detection/api/include/*.h"
   "dip/detection/detail/include/*.h"
   "utility/include/*.h"
   "device/include/*.h"
)

# conan would be preferred to get dependency, but current
# libuvc 0.0.6 is using libusb 1.0.24 which has an issue
# on macos. so we try to use system libs 0.0.6 and 1.0.25/26
# installable via home brew to overcome this issue.
# https://github.com/libusb/libusb/wiki/FAQ#How_can_I_run_libusb_applications_under_Mac_OS_X_if_there_is_already_a_kernel_extension_installed_for_the_device_and_claim_exclusive_access
set(LIBUVC_INCLUDE_DIR "") # "/usr/local/Cellar/libuvc/0.0.6/include")
set(LIBUSB_LIBRARY "") # "/usr/local/Cellar/libusb/1.0.26/lib/libusb-1.0.dylib")
set(LIBUVC_LIBRARY "") # "/usr/local/Cellar/libuvc/0.0.6/lib/libuvc.a")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CONAN_INCLUDE_DIRS_ZXING-CPP}/ZXing" ${LIBUVC_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/uic918/gen")
target_link_libraries(${PROJECT_NAME} PRIVATE ${CONAN_LIBS} ${LIBUSB_LIBRARY} ${LIBUVC_LIBRARY})

add_custom_target(${PROJECT_NAME}.Copy ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/dip/detection/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/../etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying configuration files")
