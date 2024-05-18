#!/usr/bin/env bash

set -o errexit

mkdir -p "$(pwd)"/build/ubuntuJammy.gcc11.Release
docker buildx build . -t ubuntu-jammy-gcc11-ticket-decoder-builder -f etc/ubuntuJammy.gcc11.Release.Dockerfile
docker run -it \
  --mount type=bind,source="$(pwd)"/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source="$(pwd)"/images,target=/ticket-decoder/images,readonly \
  --mount type=bind,source="$(pwd)"/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source="$(pwd)"/build/ubuntuJammy.gcc11.Release,target=/ticket-decoder/build/Release/bin \
  ubuntu-jammy-gcc11-ticket-decoder-builder
