cmake_minimum_required(VERSION 3.1.2)
project(aztec-detector)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json to make auto-complete working in vscode
cmake_policy(SET CMP0063 NEW) # To make the visibility setting below working
set(CMAKE_CXX_VISIBILITY_PRESET hidden) # Due to static linking, we get visibility warning otherwiese, since libs are using hidden und default here would lead to public

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

# add_compile_options(--std=c++20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# add_compile_options(-Wall)
# add_compile_options(-Werror)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

add_subdirectory(source/lib)
add_subdirectory(source/app)
add_subdirectory(source/test)
