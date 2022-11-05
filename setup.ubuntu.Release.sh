#!/bin/bash

docker build . -t builder -f etc/Dockerfile
docker run -it \
  --mount type=bind,source="$(pwd)"/source,target=/aztec-detector/source,readonly \
  --mount type=bind,source="$(pwd)"/CMakeLists.txt,target=/aztec-detector/CMakeLists.txt,readonly \
  --mount type=bind,source="$(pwd)"/setup.Release.sh,target=/aztec-detector/setup.Release.sh,readonly \
  builder
