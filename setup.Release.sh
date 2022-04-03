#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Release

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   conan install $DIR --build missing
   cmake $DIR -DBIN_PATH_POSTFIX=Release -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release -- -j
popd

