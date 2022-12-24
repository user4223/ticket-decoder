project(ticket-decoder-test)

aux_source_directory(source PROJECT_SOURCE)
aux_source_directory("support/source" PROJECT_SOURCE)
aux_source_directory("dip/source" PROJECT_SOURCE)
aux_source_directory("uic918/source" PROJECT_SOURCE)
aux_source_directory("barcode/source" PROJECT_SOURCE)
aux_source_directory("utility/source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

find_package(GTest)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME}
   ${CONAN_LIBS}
   ticket-decoder-lib-barcode
   ticket-decoder-lib-dip
   ticket-decoder-lib-uic918
   ticket-decoder-lib-utility)

add_custom_target(${PROJECT_NAME}.CopyUic918 ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/uic918/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/../uic918/etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying uic918 test resource files")

add_custom_target(${PROJECT_NAME}.CopyBarcode ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/barcode/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/../barcode/etc
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying barcode test resource files")
