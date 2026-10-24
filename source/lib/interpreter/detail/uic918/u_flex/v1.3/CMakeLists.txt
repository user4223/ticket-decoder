# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-interpreter-detail-uic918-u_flex13)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
AUX_SOURCE_DIRECTORY("../source" PROJECT_SOURCE)
FILE(GLOB GEN_SOURCE_DIRECTORY "gen/*.c")
FILE(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SOURCE} ${PROJECT_INCLUDES} ${GEN_SOURCE_DIRECTORY})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
   C_VISIBILITY_PRESET hidden
   CXX_VISIBILITY_PRESET hidden
   VISIBILITY_INLINES_HIDDEN ON)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:_DEFAULT_SOURCE>)
endif()
if(WIN32)
   set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
if(MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE "/FI${CMAKE_CURRENT_LIST_DIR}/../include/MsvcCompatibility.h")
endif()
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/gen")
target_link_libraries(${PROJECT_NAME} PRIVATE 
   nlohmann_json::nlohmann_json
   easyloggingpp::easyloggingpp
   ticket-decoder-utility
   ticket-decoder-infrastructure
   ticket-decoder-interpreter-detail-common)
