#!/bin/bash

# This script should be used only when a zxing-cpp version
# should be used which is not officially available via 
# https://conan.io/center/zxing-cpp

# Clones the zxing-cpp repository and builds the library 
# with a unofficial - hopefully working - conan recipe.

rm -rf etc/zxing-cpp
pushd etc
git clone -b v1.3.0 https://github.com/nu-book/zxing-cpp
cp zxing-cpp-conanfile.py zxing-cpp/conanfile.py
popd

pushd etc/zxing-cpp
conan create -s build_type=Release .
popd
