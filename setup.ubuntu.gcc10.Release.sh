#!/bin/bash

mkdir -p "$(pwd)"/build/ubuntu.gcc10.Release
docker build . -t ticket-decoder-ubuntu-gcc10-builder -f etc/ubuntu.gcc10.Dockerfile
docker run -it \
  --mount type=bind,source="$(pwd)"/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source="$(pwd)"/cert,target=/ticket-decoder/cert,readonly \
  --mount type=bind,source="$(pwd)"/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source="$(pwd)"/setup.Release.sh,target=/ticket-decoder/setup.Release.sh,readonly \
  --mount type=bind,source="$(pwd)"/build/ubuntu.gcc10.Release,target=/ticket-decoder/build/Release \
  ticket-decoder-ubuntu-gcc10-builder
