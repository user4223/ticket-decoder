# syntax=docker/dockerfile:1

FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
# Keep the following line equal 2 other Dockerfiles to make it reusable
RUN apt-get install --no-install-recommends -y cmake python-is-python3 python3-pip libgtk2.0-dev wget
RUN apt-get install --no-install-recommends -y clang-15 libc++-15-dev libc++abi-15-dev lld-15 make

ENV CC=/usr/bin/clang-15
ENV CPP=/usr/bin/clang-cpp-15
ENV CXX=/usr/bin/clang++-15
ENV LD=/usr/bin/ld.lld-15

RUN pip install conan==1.59.0
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler=clang ticket-decoder
RUN conan profile update settings.compiler.version=15 ticket-decoder
RUN conan profile update settings.compiler.libcxx=libc++ ticket-decoder

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
RUN mkdir -p cert && wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
COPY conanfile.txt .
# clang15 does not support armv8crypto intrinsics used by botan, so we have to disable for clang
RUN conan install . \
    -if build/Release \
    -pr ticket-decoder \
    -s build_type=Release \
    -o botan:with_armv8crypto=False \
    --build missing

COPY <<EOF build.sh
    #!/bin/bash
    cmake -S . -B build/Release/ -DCMAKE_BUILD_TYPE=Release
    cmake --build build/Release/ --config Release -- $@
EOF

RUN chmod 755 build.sh
