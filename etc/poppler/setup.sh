#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../

rm -rfI ${WORKSPACE_ROOT}/etc/poppler/conan-center-index

pushd ${WORKSPACE_ROOT}/etc/poppler

    git clone -b migrate/poppler https://github.com/user4223/conan-center-index.git

    conan create -s build_type=Release --build missing ./conan-center-index/recipes/poppler/all

popd
