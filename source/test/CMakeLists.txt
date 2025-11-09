project(ticket-decoder-test)

aux_source_directory(source PROJECT_SOURCE)
aux_source_directory("support/source" PROJECT_SOURCE)
aux_source_directory("dip/source" PROJECT_SOURCE)
aux_source_directory("uic918/source" PROJECT_SOURCE)
aux_source_directory("barcode/source" PROJECT_SOURCE)
aux_source_directory("utility/source" PROJECT_SOURCE)
aux_source_directory("io/source" PROJECT_SOURCE)
aux_source_directory("infrastructure/source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME}
   opencv::opencv_core_alias
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp
   gtest::gtest
   ticket-decoder-infrastructure
   ticket-decoder-io-api
   ticket-decoder-barcode
   ticket-decoder-detector-api
   ticket-decoder-dip
   ticket-decoder-interpreter-api
   ticket-decoder-interpreter-detail-common
   ticket-decoder-interpreter-detail-uic918
   ticket-decoder-utility)

add_custom_target(${PROJECT_NAME}.CopyUic918 ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/uic918/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/uic918
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying uic918 test resource files")

add_custom_target(${PROJECT_NAME}.CopyBarcode ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/barcode/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/barcode
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying barcode test resource files")

add_custom_target(${PROJECT_NAME}.CopyIo ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/io/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/io
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying io test resource files")
