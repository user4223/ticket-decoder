cmake_minimum_required(VERSION 3.22)
project(ticket-decoder)

set(CONAN_DISABLE_CHECK_COMPILER ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json to make auto-complete working in vscode
cmake_policy(SET CMP0063 NEW) # To make the visibility setting below working
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)
find_package(easyloggingpp REQUIRED)
find_package(nlohmann_json REQUIRED)

add_subdirectory(source/lib)
add_subdirectory(source/app)

IF (WITH_PYTHON_MODULE)
  add_subdirectory(source/python)
ENDIF()

add_subdirectory(source/test)
