#!/bin/bash

conan install . -of build/Debug -pr:h ticket-decoder -pr:b ticket-decoder -s build_type=Release --build missing
cmake -S . -B build/Release/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release/ --config Release -- $@
