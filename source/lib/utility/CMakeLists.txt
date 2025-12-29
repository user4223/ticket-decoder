# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-utility)

find_package(Boost REQUIRED COMPONENTS headers)

# Honestly, i don't know exactly why gcc requires visibility on for this module only.
# But maybe this is since it is the only module containing explicit template
# specializations and they make things of visibility somehow different.
set(CMAKE_CXX_VISIBILITY_PRESET default)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json
    Boost::headers)
