FROM debian:10.9

# Change apt official source to aliyun source
RUN sed -i 's/\(security\|deb\|ftp\).debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list

# Installation project dependency
RUN apt-get update && apt-get install -y cmake make g++

# Copy code into docker
WORKDIR /root/fake-media-server
COPY *.txt *.cpp /root/fake-media-server/
COPY conf/ conf/
COPY source/ source/
COPY test/ test/
COPY protocol/ protocol/

# Build Fake Media Service
RUN cmake -B build.linux -DCMAKE_BUILD_TYPE=Release && cmake --build build.linux --config Release
