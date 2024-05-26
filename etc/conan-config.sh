#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
COMPILER_NAME=${1}
COMPILER_VERSION=${2}

conan profile new ticket-decoder --force --detect

if [[ -z "${COMPILER_NAME}" ]]; then
    COMPILER_NAME=$(conan profile get settings.compiler ticket-decoder)
    if [[ -z "${COMPILER_NAME}" ]]; then
        echo "ERROR: No compiler auto-dectected or passed as argument"
        exit 1
    fi
else
    conan profile update settings.compiler=${COMPILER_NAME} ticket-decoder
fi

if [[ ! -z ${COMPILER_VERSION} ]]; then
    conan profile update settings.compiler.version=${COMPILER_VERSION} ticket-decoder
fi

readonly STANDARD_LIB=$(if [[ "$COMPILER_NAME" =~ ^(clang|apple-clang)$ ]]; then echo 'libc++'; else echo 'libstdc++11'; fi)
conan profile update settings.compiler.libcxx=${STANDARD_LIB} ticket-decoder

conan profile update conf.tools.system.package_manager:mode=install ticket-decoder
conan profile show ticket-decoder
