#!/usr/bin/env bash

set -o errexit

mkdir -p "$(pwd)"/build/ubuntuJammy.clang15.Release
docker buildx build . -t ubuntu-jammy-clang15-ticket-decoder-builder -f etc/ubuntuJammy.clang15.Release.Dockerfile
docker run -it \
  --mount type=bind,source="$(pwd)"/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source="$(pwd)"/images,target=/ticket-decoder/images,readonly \
  --mount type=bind,source="$(pwd)"/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source="$(pwd)"/build/ubuntuJammy.clang15.Release,target=/ticket-decoder/build/Release/bin \
  ubuntu-jammy-clang15-ticket-decoder-builder
