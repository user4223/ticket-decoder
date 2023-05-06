FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
# Keep the following line equal 2 other Dockerfiles to make it reusable
RUN apt-get install --no-install-recommends -y cmake python-is-python3 python3-pip libgtk2.0-dev wget
RUN apt-get install --no-install-recommends -y build-essential
# Desired gcc11 is default in jammy, but when the desired compiler version changes, do something like the following to change
# RUN apt-get install -y gcc-10 g++-10 cpp-10 libstdc++-10-dev
# RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10

RUN pip install conan==1.59.0
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 ticket-decoder

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.txt ./conanfile.txt
RUN conan install -if build/Release -pr ticket-decoder -pr:b ticket-decoder -s build_type=Release --build missing .

RUN mkdir -p cert && wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
