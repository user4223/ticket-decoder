#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Release
RETURN_CODE=0

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   conan install $DIR -s build_type=Release --build missing
   cmake -S $DIR -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release -- -j
   RETURN_CODE=$?
popd

exit $RETURN_CODE
