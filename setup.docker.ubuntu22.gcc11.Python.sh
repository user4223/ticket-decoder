#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"

mkdir -p ${WORKSPACE_ROOT}/build/ubuntu22.gcc11.Python
docker buildx build ${WORKSPACE_ROOT} -t ubuntu22-gcc11-ticket-decoder-python-builder -f etc/docker/ubuntu22.gcc.Python.Dockerfile  --build-arg="GCC_VERSION=11"
docker run -it \
  --mount type=bind,source=${WORKSPACE_ROOT}/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/images,target=/ticket-decoder/images,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/build/ubuntu22.gcc11.Python,target=/ticket-decoder/build/Release/bin \
  ubuntu22-gcc11-ticket-decoder-python-builder
