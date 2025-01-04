#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
COMPILER_NAME=${1}
COMPILER_VERSION=${2}

conan profile detect --name ticket-decoder --force

#if [[ ! -z "${COMPILER_NAME}" ]]; then
#    conan profile show -pr ticket-decoder -s compiler=${COMPILER_NAME}
#fi

#if [[ ! -z ${COMPILER_VERSION} ]]; then
#    conan profile show -pr ticket-decoder -s compiler.version=${COMPILER_VERSION}
#fi

#conan profile show -pr ticket-decoder -c tools.system.package_manager:mode=install
