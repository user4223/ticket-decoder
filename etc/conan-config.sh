#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly COMPILER_NAME=${1}
readonly COMPILER_VERSION=${2}
readonly STANDARD_LIB=$(if [[ "$COMPILER_NAME" == "clang" ]]; then echo 'libc++'; else echo 'libstdc++11'; fi)

conan profile new ticket-decoder --force --detect

conan profile update settings.compiler=${COMPILER_NAME} ticket-decoder
conan profile update settings.compiler.version=${COMPILER_VERSION} ticket-decoder
conan profile update settings.compiler.libcxx=${STANDARD_LIB} ticket-decoder
conan profile update conf.tools.system.package_manager:mode=install ticket-decoder

conan profile show ticket-decoder
