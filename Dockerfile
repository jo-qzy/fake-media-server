FROM debian:10.9 as builder

# Change apt official source to aliyun source
RUN sed -i 's/\(security\|deb\|ftp\).debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list

# Installation project dependency
RUN apt-get update && \
    apt-get -y install cmake make g++

# Copy code into docker
WORKDIR /root/compile_project
COPY *.txt *.cpp /root/compile_project/
COPY source/ source/
COPY test/ test/
COPY protocol/ protocol/

# Build Fake Media Service
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release
COPY conf/ /root/fake-media-server/conf/
RUN mkdir -p /root/fake-media-server/bin && cp build/fake-media-server /root/fake-media-server/bin

FROM debian:10.9

COPY --from=builder /root/fake-media-server /data/app/fake-media-server
CMD ["/data/app/fake-media-server/bin/fake-media-server", "-c", "/data/app/fake-media-server/conf/fake.conf"]
