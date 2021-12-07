//
// Created by quzhenyu on 2021/12/7.
// Copyright (c) quzhenyu. All rights reserved.
//

#include <flv_demuxer.h>
#include <flv_reader.h>
#include <flv_type.h>

#include <iostream>
#include <fstream>
#include <vector>

int callback(void *param, int codec, const void *data, uint32_t bytes,
             uint32_t pts, uint32_t dts, int flags)
{
    auto file = (std::fstream *) param;

    switch (codec) {
        case FLV_VIDEO_H264:
            std::cout << "Demuxer h264 video: " << bytes << std::endl;
            file->write(static_cast<const char *>(data), bytes);
            break;
    }

    return 0;
}

int main()
{
    std::fstream h264_file;
    std::vector<uint8_t> flv_stream;
    int ret, tag_type;
    uint32_t timestamp, tag_len;

    flv_stream.resize(2 * 1024 * 1024);
    h264_file.open("test.h264", std::fstream::out | std::ios::binary);

    flv_reader_t *reader = flv_reader_create("test.flv", nullptr, nullptr);
    flv_demuxer_t *demuxer = flv_demuxer_create(callback, &h264_file);

    while (1 == flv_reader_read(reader, &tag_type, &timestamp, &tag_len, flv_stream.data(), (uint32_t) flv_stream.size())) {
        ret = flv_demuxer_input(demuxer, tag_type, flv_stream.data(), tag_len, timestamp);
    }

    return 0;
}
