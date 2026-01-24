# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-interpreter-detail-vdv)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   easyloggingpp::easyloggingpp
   nlohmann_json::nlohmann_json
   ticket-decoder-interpreter-detail-common
   ticket-decoder-utility)

# TODO This is just a temporary solution to solve compiling issues with botan3 and gcc11.
#      gcc11.4 is the default compiler coming with ubuntu22, but conan build of botan3
#      is complaining about missing c++20 support and states a minimum required gcc11.2.
#      Maybe this is just an issue in conan recipe of botan3. But it could be a failing
#      test during build about a specific c++20 feature as well, just failing with gcc11.4
#      and the message about minimum required version is just outdated.
IF (WITH_SIGNATURE_VERIFIER)

   find_package(botan REQUIRED)
   target_link_libraries(${PROJECT_NAME} PRIVATE botan::botan)

ENDIF()
