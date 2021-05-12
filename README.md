# Fake Media Server

Just a small project use for learning media server.

- Fake Media Server **cannot use in any online project**
- Fake Media Server **performance is very poor**
- Fake Media Server **does not use third party libraries (except OpenSSL)**
- Fake Media Server **follow the principle of writing the simplest code possible**

# Feature

None of the following features are supported, to be done...

- [ ] An event-driven server
- [ ] Support echo protocol
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
mkdir build
cd build
cmake ..
make -j
make install
```

# Enjoy It

```shell
./bin/fake_media_server -c conf/fake.conf
```

