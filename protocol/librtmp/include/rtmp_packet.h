//
// Created by BoringWednesday on 2021/6/14.
//

#ifndef LIBRTMP_PACKET_H
#define LIBRTMP_PACKET_H

#include <stdint.h>
#include <stddef.h>
#include "rtmp.h"

#define RTMP_MAX_CHUNK_STREAM       8
#define RTMP_CHUNK_HEADER_MAX_LEN   18

enum rtmp_chunk_type_t
{
    RTMP_CHUNK_TYPE_0 = 0,
    RTMP_CHUNK_TYPE_1 = 1,
    RTMP_CHUNK_TYPE_2 = 2,
    RTMP_CHUNK_TYPE_3 = 3,
};

const uint32_t rtmp_chunk_header_length[] = { 12, 8, 4, 1 };

/* RTMP Chunk Header
 *
 * 1. Chunk Format = 0 (00)
 *    Chunk Header Len = 12-18 bytes
 *  | 2 bits |    6-22 bits    |  3 bytes  |    3 bytes     |     1 bytes     |    4 bytes (LE)   |
 *  | format | chunk stream ID | timestamp | message length | message type ID | message stream ID |
 *
 * 2. Chunk Format = 1 (01)
 *    Chunk Header Len = 8-14 bytes
 *  | 2 bits |    6-22 bits    |  3 bytes  |    3 bytes     |     1 bytes     |
 *  | format | chunk stream ID | timestamp | message length | message type ID |
 *
 * 3. Chunk Format = 2 (10)
 *    Chunk Header Len = 4-10 bytes
 *  | 2 bits |    6-22 bits    |  3 bytes  |
 *  | format | chunk stream ID | timestamp |
 *
 * 4. Chunk Format = 3 (11)
 *    Chunk Header Len = 1-3 bytes
 *  | 2 bits |    6-22 bits    |
 *  | format | chunk stream ID |
 *
 *  Max Chunk Header example (18 bytes):
 *  | 2 bits |     22 bits     |  3 bytes  |    3 bytes     |     1 bytes     |      4 bytes      |    4 bytes    |
 *  | format | chunk stream ID | timestamp | message length | message type ID | message stream ID | ext timestamp |
 *
 *  About Chunk Stream ID:
 *   Chunk Stream ID can range in length from 6 to 22 bits, it depends on the first 6 bits value
 *      1. If first 6 bits = 0, the length is 14 bits
 *          chunk stream ID = value(1 bytes) + 64
 *      2. If first 6 bits = 1, the length is 22 bits
 *          chunk stream ID = value(2 bytes) + 64
 *      3. If first 6 bits != 0 or 1, the length is 6 bits
 *          chunk stream ID = value(6 bits)
 *
 * About timestamp:
 *  If timestamp is bigger than 0xFFFFFF, then need 4 bytes extended timestamp, the extended
 *  timestamp will appear after the header.
 *
 * About Message Stream ID:
 *  Attention!!!! It is LITTLE ENDIAN!!!
 *
 */

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


int rtmp_chunk_read(struct rtmp_t *rtmp, const uint8_t *data, size_t bytes);

#endif //LIBRTMP_PACKET_H
