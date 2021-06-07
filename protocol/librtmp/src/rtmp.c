//
// Created by BoringWednesday on 2021/5/12.
//

#include <stdint.h>
#include "rtmp.h"

#define RTMP_CHUNK_HEADER_MAX_LEN 18

enum rtmp_chunk_type_t
{
    RTMP_CHUNK_TYPE_0 = 0,  //
    RTMP_CHUNK_TYPE_1 = 1,  //
    RTMP_CHUNK_TYPE_2 = 2,  //
    RTMP_CHUNK_TYPE_3 = 3,  //
};

/* RTMP Chunk Header
 *
 * 1. Chunk Format = 0 (00)
 *    Chunk Header Len = 12-18 bytes
 *  | 2 bits |    6-22 bits    | 3-7 bytes |    3 bytes     |     1 bytes     |      4 bytes      |
 *  | format | chunk stream ID | timestamp | message length | message type ID | message stream ID |
 *
 * 2. Chunk Format = 1 (01)
 *    Chunk Header Len = 8-14 bytes
 *  | 2 bits |    6-22 bits    | 3-7 bytes |    3 bytes     |     1 bytes     |
 *  | format | chunk stream ID | timestamp | message length | message type ID |
 *
 * 3. Chunk Format = 2 (10)
 *    Chunk Header Len = 4-10 bytes
 *  | 2 bits |    6-22 bits    | 3-7 bytes |
 *  | format | chunk stream ID | timestamp |
 *
 * 4. Chunk Format = 2 (11)
 *    Chunk Header Len = 1-3 bytes
 *  | 2 bits |    6-22 bits    |
 *  | format | chunk stream ID |
 *
 * ATTENTION:
 *  1. If format followed by 6 bits is equal to 1, then the length of the
 *     chunk stream ID is 22 bits, otherwise it is 6 bits
 *  2. If timestamp is bigger than 0xFFFFFF, then need 4 bytes extended
 *     timestamp, timestamp length is 7 bytes
 *
 */

struct rtmp_chunk_header_t
{
    uint8_t     format;             // RTMP_CHUNK_TYPE_X
    uint32_t    chunk_stream_id;    // chunk stream ID (6-bits or 22-bits)
    uint32_t    timestamp;          // timestamp (24-bits)
    uint32_t    extended_timestamp; // extended timestamp(32-bits)
    uint32_t    message_length;     // message length (24-bits)
    uint8_t     message_type_id;    // message type id
    uint32_t    message_stream_id;  // message stream id
};

struct rtmp_packet_t
{
    struct rtmp_chunk_header_t header;

};

struct rtmp_t
{
    int         in_chunk_size;
    int         out_chunk_size;
};
