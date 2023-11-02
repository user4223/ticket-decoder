#!/bin/bash

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
conan install . -if build/Release -pr ticket-decoder -s build_type=Release --build missing
cmake -S . -B build/Release/ -DCMAKE_TOOLCHAIN_FILE=build/Release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release/ --config Release -- $@
