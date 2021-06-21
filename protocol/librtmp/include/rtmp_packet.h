//
// Created by BoringWednesday on 2021/6/14.
//

#ifndef LIBRTMP_PACKET_H
#define LIBRTMP_PACKET_H

#include <stdint.h>
#include <stddef.h>

#define RTMP_MAX_CHUNK_STREAM       8

#define RTMP_CHUNK_HEADER_MAX_LEN   18
#define RTMP_CHUNK_SIZE             128

enum rtmp_chunk_type_t
{
    RTMP_CHUNK_TYPE_0 = 0,
    RTMP_CHUNK_TYPE_1 = 1,
    RTMP_CHUNK_TYPE_2 = 2,
    RTMP_CHUNK_TYPE_3 = 3,
};

const uint32_t rtmp_chunk_header_length[] = { 12, 8, 4, 1 };

struct rtmp_chunk_header_t
{
    /* 2 bits. Chunk header format */
    uint8_t     format;

    /* 6 bits. Chunk stream ID, maybe 6, 14 or 22 bits */
    uint32_t    chunk_stream_id;

    /* 3 bytes. The timestamp of packet, maybe delta timestamp */
    /* If timestamp == 0xFFFFFF, the value means 4 bytes extended timestamp */
    uint32_t    timestamp;

    /* 3 bytes. The length of message */
    uint32_t    message_length;

    /* 1 bytes. The type of the message */
    uint8_t     message_type_id;

    /* 4 bytes. Little Endian. The ID of message */
    uint32_t    message_stream_id;
};

struct rtmp_packet_t
{
    /* Header of packet */
    struct rtmp_chunk_header_t header;

    /* Delta of timestamp */
    uint32_t    delta_timestamp;

    /* The RTMP packet real payload */
    uint8_t    *payload;

    /* Max buffer capacity of payload */
    size_t      capacity;
};


#endif //LIBRTMP_PACKET_H
