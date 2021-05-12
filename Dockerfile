FROM debian:10.9

# 更换中科大的源
RUN sed -i 's/\(security\|deb\).debian.org/mirrors.aliyun.com/g' /etc/apt/sources.list

# 安装cmake
RUN apt-get update && apt-get install -y cmake make g++

# 构建服务
WORKDIR /root/fake-media-server
COPY *.txt *.cpp /root/fake-media-server/
COPY conf/ conf/
COPY source/ source/
COPY test/ test/
COPY protocol/ protocol/
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release
