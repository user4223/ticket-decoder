#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Debug

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   conan install $DIR --build missing
   cmake $DIR -DBIN_PATH_POSTFIX=Debug -DCMAKE_BUILD_TYPE=Debug
   cmake --build . --config Debug -- -j
popd

