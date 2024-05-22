# syntax=docker/dockerfile:1

FROM ubuntu:22.04

ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends make cmake wget python-is-python3 python3-pip python3-dev

# Keep all commands above equal in all build container docker files to make layers re-usable

RUN apt-get install -y --no-install-recommends build-essential libgtk2.0-dev

# Desired gcc11 is default in jammy, but when the desired compiler version changes, do something like the following to change
# RUN apt-get install --no-install-recommends -y gcc-10 g++-10 cpp-10 libstdc++-10-dev make

# RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 800
# RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 800

RUN pip install conan==1.64.0 numpy
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 ticket-decoder
RUN conan profile update conf.tools.system.package_manager:mode=install ticket-decoder

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.py .
COPY etc/conan-install.sh etc/cmake-config.sh etc/cmake-build.sh etc/python-test.sh etc/install-uic-keys.sh etc/
RUN etc/conan-install.sh Release
COPY <<EOF /ticket-decoder/build.sh
    #!/usr/bin/env bash

    set -o errexit

    ./etc/cmake-config.sh Release
    ./etc/cmake-build.sh Release \$\@
EOF
RUN chmod 755 build.sh
RUN etc/install-uic-keys.sh

ENV PYTHONPATH=/ticket-decoder/build/Release/bin
