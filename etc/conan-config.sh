#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../

# ensure we do have a default profile, when it already exists, it stays and
# the detect calls fails and gets ignored
conan profile detect || true

# install compiler specific settings intentionally to ensure we are using
# exactly the desired compiler, version and std-lib
conan config install -tf profiles/ -t dir ${WORKSPACE_ROOT}etc/conan/profiles/
