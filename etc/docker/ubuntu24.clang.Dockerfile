# syntax=docker/dockerfile:1

FROM ubuntu:24.04

ARG CLANG_VERSION
ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends make cmake wget python-is-python3 python3-pip python3-dev python3-venv git
# Keep all commands above equal in all build container docker files to make layers re-usable
RUN apt-get install -y --no-install-recommends clang-$CLANG_VERSION libc++-$CLANG_VERSION-dev libc++abi-$CLANG_VERSION-dev lld-$CLANG_VERSION libgtk2.0-dev
RUN apt-get clean

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-$CLANG_VERSION 800
RUN update-alternatives --install /usr/bin/g++     g++     /usr/bin/clang++-$CLANG_VERSION 800
RUN update-alternatives --install /usr/bin/c++     c++     /usr/bin/clang++-$CLANG_VERSION 800
RUN update-alternatives --install /usr/bin/clang   clang   /usr/bin/clang-$CLANG_VERSION   800
RUN update-alternatives --install /usr/bin/gcc     gcc     /usr/bin/clang-$CLANG_VERSION   800
RUN update-alternatives --install /usr/bin/cc      cc      /usr/bin/clang-$CLANG_VERSION   800
RUN update-alternatives --install /usr/bin/ld.lld  lld     /usr/bin/ld.lld-$CLANG_VERSION  800
RUN update-alternatives --install /usr/bin/ld      ld      /usr/bin/ld.lld-$CLANG_VERSION  800

WORKDIR /ticket-decoder
COPY etc/conan-config.sh etc/conan-install.sh etc/cmake-config.sh etc/cmake-build.sh etc/python-test.sh etc/install-uic-keys.sh etc/
COPY etc/poppler/ etc/poppler

RUN python3 -m venv .venv && \
    . .venv/bin/activate && \
    pip install --upgrade pip && \
    pip install "conan<2.0" "numpy" jsonpath2
ENV PATH="/ticket-decoder/.venv/bin:$PATH"

RUN etc/conan-config.sh clang $CLANG_VERSION

COPY conanfile.py .
# clang does not support armv8crypto intrinsics used by botan, so we have to disable for clang on arm
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
