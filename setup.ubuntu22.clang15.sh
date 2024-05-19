#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(dirname "$0")"

mkdir -p ${WORKSPACE_ROOT}/build/ubuntu22.clang15.Release
docker buildx build ${WORKSPACE_ROOT} -t ubuntu22-clang15-ticket-decoder-builder -f etc/ubuntu22.clang15.Dockerfile
docker run -it \
  --mount type=bind,source=${WORKSPACE_ROOT}/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/images,target=/ticket-decoder/images,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/build/ubuntu22.clang15.Release,target=/ticket-decoder/build/Release/bin \
  ubuntu22-clang15-ticket-decoder-builder
