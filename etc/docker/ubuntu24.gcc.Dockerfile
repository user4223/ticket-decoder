# syntax=docker/dockerfile:1

FROM ubuntu:24.04

ARG GCC_VERSION
ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends make cmake wget python-is-python3 python3-pip python3-dev python3-venv git
# Keep all commands above equal in all build container docker files to make layers re-usable
RUN apt-get install -y --no-install-recommends gcc-$GCC_VERSION g++-$GCC_VERSION cpp-$GCC_VERSION libstdc++-$GCC_VERSION-dev libgtk2.0-dev
RUN apt-get clean

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-$GCC_VERSION 800
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-$GCC_VERSION 800
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-$GCC_VERSION 800
RUN update-alternatives --install /usr/bin/cc  cc  /usr/bin/gcc-$GCC_VERSION 800

WORKDIR /ticket-decoder
COPY etc/conan-config.sh etc/conan-install.sh etc/cmake-config.sh etc/cmake-build.sh etc/python-test.sh etc/install-uic-keys.sh etc/
COPY etc/poppler/ etc/poppler
COPY etc/conan/profiles etc/conan/profiles

RUN python3 -m venv venv && \
    . venv/bin/activate && \
    pip install -r requirements.txt
ENV PATH="/ticket-decoder/venv/bin:$PATH"

RUN etc/conan-config.sh gcc $GCC_VERSION

COPY conanfile.py .
RUN etc/conan-install.sh Release \
    -pr:a ./etc/conan/profiles/ubuntu24 \
    -o libxml2/*:zlib=False

COPY <<EOF /ticket-decoder/build.sh
    #!/usr/bin/env bash

    set -o errexit

    ./etc/cmake-config.sh Release
    ./etc/cmake-build.sh Release \$\@
EOF
RUN chmod 755 build.sh
RUN etc/install-uic-keys.sh

ENV PYTHONPATH=/ticket-decoder/build/Release/bin
