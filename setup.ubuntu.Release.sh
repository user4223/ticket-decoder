#!/bin/bash

docker build . -t ticket-decoder-builder -f etc/Dockerfile
docker run -it \
  --mount type=bind,source="$(pwd)"/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source="$(pwd)"/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source="$(pwd)"/setup.Release.sh,target=/ticket-decoder/setup.Release.sh,readonly \
  ticket-decoder-builder
