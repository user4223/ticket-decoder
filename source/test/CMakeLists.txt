# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

project(ticket-decoder-test)

aux_source_directory("support/source" PROJECT_SOURCE)
aux_source_directory("decoder/source" PROJECT_SOURCE)
aux_source_directory("detector/source" PROJECT_SOURCE)
aux_source_directory("dip/source" PROJECT_SOURCE)
aux_source_directory("infrastructure/source" PROJECT_SOURCE)
aux_source_directory("interpreter/source" PROJECT_SOURCE)
aux_source_directory("io/source" PROJECT_SOURCE)
aux_source_directory("utility/source" PROJECT_SOURCE)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   opencv::opencv_core_alias
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp
   gtest::gtest
   ticket-decoder-infrastructure
   ticket-decoder-input-api
   ticket-decoder-output-api
   ticket-decoder-decoder-api
   ticket-decoder-detector-api
   ticket-decoder-dip
   ticket-decoder-interpreter-api
   ticket-decoder-interpreter-detail-common
   ticket-decoder-utility)

if (WITH_UIC_INTERPRETER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-uic918)
ENDIF()

add_custom_target(${PROJECT_NAME}.CopyInterpreter ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/interpreter/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/interpreter
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying interpreter test resource files")

add_custom_target(${PROJECT_NAME}.CopyDecoder ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/decoder/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/decoder
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying decoder test resource files")

add_custom_target(${PROJECT_NAME}.CopyIo ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/io/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/io
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying io test resource files")
