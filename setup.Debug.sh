#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Debug
RETURN_CODE=0

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   conan install $DIR -s build_type=Debug --build missing -pr=debug
   cmake -S $DIR -DCMAKE_BUILD_TYPE=Debug
   cmake --build . --config Debug -- $@
   RETURN_CODE=$?
popd

exit $RETURN_CODE
