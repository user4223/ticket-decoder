#!/bin/bash

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
conan install . -if build/Release -pr ticket-decoder -pr:b ticket-decoder -s build_type=Release --build missing -o:h with_analyzer=False
# when presets are working for sure due to cmake >= 3.23, use: --preset release
cmake -S . -B build/Release/ -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release/ --config Release -t ticket_decoder -- $@

export PYTHONPATH=`pwd`/build/Release/bin
