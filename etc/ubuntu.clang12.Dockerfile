FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install -y cmake python3-pip libgtk2.0-dev
RUN apt-get install -y clang-12 libc++-12-dev libc++abi-12-dev lld-12 

ENV CC=/usr/bin/clang-12
ENV CPP=/usr/bin/clang-cpp-12
ENV CXX=/usr/bin/clang++-12
ENV LD=/usr/bin/ld.lld-12

RUN pip install conan
RUN conan profile new default --detect
RUN conan profile update settings.compiler=clang default
RUN conan profile update settings.compiler.version=12 default
RUN conan profile update settings.compiler.libcxx=libc++ default

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.txt ./conanfile.txt

WORKDIR /ticket-decoder/build/Release
RUN conan install /ticket-decoder/ -s build_type=Release --build missing

WORKDIR /ticket-decoder