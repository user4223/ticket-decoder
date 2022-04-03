cmake_minimum_required(VERSION 3.1.2)
project(aztec-detector)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

add_compile_options(--std=c++14)
add_compile_options(-Wall)
add_compile_options(-Werror)

include(${CMAKE_BINARY_DIR}/../conan/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

add_subdirectory(source/lib)
add_subdirectory(source/app)
add_subdirectory(source/test)

