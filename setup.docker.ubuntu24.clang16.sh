#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"

mkdir -p ${WORKSPACE_ROOT}/build/ubuntu24.clang16.Release
docker buildx build ${WORKSPACE_ROOT} -t ubuntu24-clang16-ticket-decoder-builder -f etc/docker/ubuntu24.clang.Dockerfile --build-arg="CLANG_VERSION=16"
docker run -it \
  --mount type=bind,source=${WORKSPACE_ROOT}/source,target=/ticket-decoder/source,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/images,target=/ticket-decoder/images,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/CMakeLists.txt,target=/ticket-decoder/CMakeLists.txt,readonly \
  --mount type=bind,source=${WORKSPACE_ROOT}/build/ubuntu24.clang16.Release,target=/ticket-decoder/build/Release/bin \
  ubuntu24-clang16-ticket-decoder-builder
