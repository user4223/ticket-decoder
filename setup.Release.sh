#!/bin/bash

conan install . -if build/Release -s build_type=Release --build missing
cmake -S . -B build/Release/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release/ --config Release -- $@
