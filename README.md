# Fake Media Server

Just a small project use for learning media server.

- Fake Media Server **cannot use in any online project**
- Fake Media Server **performance is very poor**
- Fake Media Server **does not use third party libraries (except OpenSSL)**
- Fake Media Server **follow the principle of writing the simplest code possible**

# Feature

Most of the features listed below are not supported, to be done...

- [x] An event-driven server
- [x] Support echo protocol

## Todo-List

- [ ] Media remux (like RTMP Publish, WebRTC pull)
- [ ] Clustering support
- [ ] Publish media by RTMP
- [ ] Publish media by WebRTC
- [ ] Publish media by SRT
- [ ] Play media by RTMP
- [ ] Play media by HTTP-HLS (support both TS and fmp4)
- [ ] Play media by HTTP-FLV
- [ ] Play media by WebRTC

# How To Build

```shell
# Release version
cmake -B build -DCMAKE_BUILD_TYPE="Release"
cmake --build build --config "Release"
cmake -P build/cmake_install.cmake

# Debug version
cmake -B build -DCMAKE_BUILD_TYPE="Debug"
cmake --build build --config "Debug"
cmake -P build/cmake_install.cmake
```

# Enjoy It

```shell
./bin/fake_media_server -c conf/fake.conf
```

# Directory description

| Directory | Instructions                                                 |
| :-------: | :----------------------------------------------------------- |
|  common   | Server base component code, including **network/protocol adapter/common tool** (like log, IO buffer, signal handle) code, most core code is in here. |
|   conf    | Server configuration example directory.                      |
|    doc    | Document of media format and protocol standard.              |
| protocol  | Protocol and media format implementation source code.        |
|  scripts  | Include some scripts to create development environments and build servers easier. |
|  source   | Server implementation code.                                  |
|   test    | Include some unit test.                                      |

# In Addition

I borrowed a lot of GitHub user [ireader](https://github.com/ireader/media-server) 's code in my code, thanks a lot for open-source the excellent code.
