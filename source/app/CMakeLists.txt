# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

project(ticket-decoder)

find_package(tclap REQUIRED)

file(GLOB PROJECT_INCLUDES "include/*.h")

IF (WITH_TICKET_ANALYZER)
  add_executable("ticket-analyzer" "source/analyzer.cpp" "source/InteractionController.cpp" ${PROJECT_INCLUDES})
  target_include_directories("ticket-analyzer" PRIVATE)
  target_link_libraries("ticket-analyzer" PRIVATE
    tclap::tclap
    opencv::opencv_core_alias
    easyloggingpp::easyloggingpp
    nlohmann_json::nlohmann_json
    ticket-decoder-api
    ticket-decoder-output-api
    ticket-decoder-utility
    ticket-decoder-ui)
ENDIF()

IF (WITH_TICKET_DECODER)
  add_executable("ticket-decoder" "source/decoder.cpp" ${PROJECT_INCLUDES})
  target_include_directories("ticket-decoder" PRIVATE)
  target_link_libraries("ticket-decoder" PRIVATE
    tclap::tclap
    opencv::opencv_core_alias
    easyloggingpp::easyloggingpp
    ticket-decoder-api
    ticket-decoder-output-api
    ticket-decoder-dip)
ENDIF()
