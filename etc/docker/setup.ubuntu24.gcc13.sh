#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../

mkdir -p ${WORKSPACE_ROOT}/build/ubuntu24.gcc13.Release

docker buildx build ${WORKSPACE_ROOT} \
  -t ubuntu24-gcc13-ticket-decoder-builder \
  -f ${WORKSPACE_ROOT}/etc/docker/ubuntu24.gcc.Dockerfile \
  --build-arg="GCC_VERSION=13"

docker run -it \
  --mount type=bind,source=${WORKSPACE_ROOT}/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/images,target=/ticket-decoder/images,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/build/ubuntu24.gcc13.Release,target=/ticket-decoder/build/Release/bin \
  ubuntu24-gcc13-ticket-decoder-builder
