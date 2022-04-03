#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Release
CONAN_BUILD_DIR=$DIR/build/conan

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   conan install $DIR --install-folder $CONAN_BUILD_DIR --build missing
   cmake $DIR -DBIN_PATH_POSTFIX=Release -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release -- -j
popd

