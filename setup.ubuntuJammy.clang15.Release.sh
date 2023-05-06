#!/bin/bash

mkdir -p "$(pwd)"/build/ubuntuJammy.clang15.Release
docker build . -t ubuntu-jammy-clang15-ticket-decoder-builder -f etc/ubuntuJammy.clang15.Release.Dockerfile
docker run -it \
  --mount type=bind,source="$(pwd)"/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source="$(pwd)"/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source="$(pwd)"/setup.Release.sh,target=/ticket-decoder/setup.Release.sh,readonly \
  --mount type=bind,source="$(pwd)"/build/ubuntuJammy.clang15.Release,target=/ticket-decoder/build/Release \
  ubuntu-jammy-clang15-ticket-decoder-builder
