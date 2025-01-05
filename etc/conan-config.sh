#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
COMPILER_NAME=${1}
COMPILER_VERSION=${2}

# ensure we do have a default profile, when it already exists, it stays and
# the detect calls fails and gets ignored
conan profile detect || true

# install compiler specific settings intentionally to ensure we are using
# exactly the desired compiler, version and std-lib
conan config install -t file -sf ${WORKSPACE_ROOT}/etc/conan/${COMPILER_NAME}${COMPILER_VERSION}/ -tf profiles/ ticket-decoder
