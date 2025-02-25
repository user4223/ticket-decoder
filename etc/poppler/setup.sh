#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../

pushd ${WORKSPACE_ROOT}/etc/poppler

    rm -rf conan-center-index
    git clone -b migrate/poppler https://github.com/valgur/conan-center-index
    # conan create -s build_type=Release --build missing ./conan-center-index/recipes/poppler/all
    conan remote add valgur ${WORKSPACE_ROOT}/etc/poppler/conan-center-index --allowed-packages="poppler/*" --force

popd
