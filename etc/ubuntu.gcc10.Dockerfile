FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install -y cmake python3-pip libgtk2.0-dev
RUN apt-get install -y gcc-10 g++-10 cpp-10 libstdc++-10-dev
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10

RUN pip install conan
RUN conan profile new default --detect
RUN conan profile update settings.compiler=gcc default
RUN conan profile update settings.compiler.version=10 default
RUN conan profile update settings.compiler.libcxx=libstdc++11 default

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.txt ./conanfile.txt

WORKDIR /ticket-decoder/build/Release
RUN conan install /ticket-decoder/ -s build_type=Release --build missing

WORKDIR /ticket-decoder