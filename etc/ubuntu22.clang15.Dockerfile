# syntax=docker/dockerfile:1

FROM ubuntu:22.04

ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends make cmake wget python-is-python3 python3-pip python3-dev
# Keep all commands above equal in all build container docker files to make layers re-usable
RUN apt-get install -y --no-install-recommends clang-15 libc++-15-dev libc++abi-15-dev lld-15 libgtk2.0-dev
RUN apt-get clean

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-15 800
RUN update-alternatives --install /usr/bin/clang   clang   /usr/bin/clang-15   800
RUN update-alternatives --install /usr/bin/ld.lld  lld     /usr/bin/ld.lld-15  800
RUN update-alternatives --install /usr/bin/cc      cc      /usr/bin/clang-15   800
RUN update-alternatives --install /usr/bin/c++     c++     /usr/bin/clang++-15 800
RUN update-alternatives --install /usr/bin/ld      ld      /usr/bin/ld.lld-15  800

WORKDIR /ticket-decoder
COPY etc/conan-config.sh etc/conan-install.sh etc/cmake-config.sh etc/cmake-build.sh etc/python-test.sh etc/install-uic-keys.sh etc/

RUN pip install conan==1.64.0 numpy
RUN conan profile new ticket-decoder --force --detect
RUN etc/conan-config.sh clang 15
RUN conan profile update conf.tools.system.package_manager:mode=install ticket-decoder

COPY conanfile.py .
# clang15 does not support armv8crypto intrinsics used by botan, so we have to disable for clang on arm
RUN echo $TARGETARCH
RUN etc/conan-install.sh Release $(if [ "$TARGETARCH" = "arm64" ]; then echo '-o botan:with_armv8crypto=False'; fi)
COPY <<EOF build.sh
    #!/usr/bin/env bash

    set -o errexit

    ./etc/cmake-config.sh Release
    ./etc/cmake-build.sh Release \$\@
EOF
RUN chmod 755 build.sh
RUN etc/install-uic-keys.sh

ENV PYTHONPATH=/ticket-decoder/build/Release/bin
