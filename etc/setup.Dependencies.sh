#!/bin/bash

git clone -b v1.2.0 https://github.com/nu-book/zxing-cpp
cp conanfile.py zxing-cpp/
cd zxing-cpp
conan create .

