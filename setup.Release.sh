#!/bin/bash

conan install . -if build/Release -pr ticket-decoder -s build_type=Release --build missing
cmake -S . -B build/Release/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release/ --config Release -- $@
