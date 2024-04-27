# syntax=docker/dockerfile:1

FROM ubuntu:22.04

ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install --no-install-recommends -y cmake python-is-python3 python3-pip python3-dev libgtk2.0-dev wget

# Keep all commands above equal in all build container docker files to make layers re-usable

RUN apt-get install --no-install-recommends -y clang-15 libc++-15-dev libc++abi-15-dev lld-15 make

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-15 800
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-15 800
RUN update-alternatives --install /usr/bin/ld.lld lld /usr/bin/ld.lld-15 800
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang-15 800
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-15 800
RUN update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-15 800

RUN pip install conan==1.64.0 numpy
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler=clang ticket-decoder
RUN conan profile update settings.compiler.version=15 ticket-decoder
RUN conan profile update settings.compiler.libcxx=libc++ ticket-decoder
RUN conan profile update conf.tools.system.package_manager:mode=install ticket-decoder

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
RUN mkdir -p cert && wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
COPY conanfile.py .

# clang15 does not support armv8crypto intrinsics used by botan, so we have to disable for clang on arm
RUN echo $TARGETARCH
RUN conan install . \
    -if build/Release \
    -pr ticket-decoder \
    -pr:b ticket-decoder \
    -s build_type=Release \
    --build missing \
    $(if [ "$TARGETARCH" = "arm64" ]; then echo '-o botan:with_armv8crypto=False'; fi)

COPY <<EOF build.sh
    #!/bin/bash
    # cmake 3.22 is not supporting presets, so we have to use toolchain file: https://docs.conan.io/2.0/examples/tools/cmake/cmake_toolchain/build_project_cmake_presets.html#building-the-project-using-cmakepresets
    cmake -S . -B build/Release/ -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build build/Release/ --config Release -- $@
EOF

RUN chmod 755 build.sh
