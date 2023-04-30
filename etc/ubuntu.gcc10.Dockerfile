FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install -y cmake python-is-python3 python3-pip libgtk2.0-dev
RUN apt-get install -y gcc-10 g++-10 cpp-10 libstdc++-10-dev
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10

RUN pip install conan==1.59.0
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler=gcc ticket-decoder
RUN conan profile update settings.compiler.version=10 ticket-decoder
RUN conan profile update settings.compiler.libcxx=libstdc++11 ticket-decoder

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.txt ./conanfile.txt
RUN conan install . -if build/Release -pr ticket-decoder -s build_type=Release --build missing
