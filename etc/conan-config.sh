#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../

# ensure we do have a default profile, when it already exists, it stays and
# the detect calls fails and gets ignored
conan profile detect 2>/dev/null 1>&2 || true

# install compiler specific settings intentionally to ensure we are using
# exactly the desired compiler, version and std-lib.
# but right now, we use the profiles with relative paths directly from etc/conan/profiles,
# so no need to install them at user home.
# conan config install -tf profiles/ -t dir ${WORKSPACE_ROOT}etc/conan/profiles/
