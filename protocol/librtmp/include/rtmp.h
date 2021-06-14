//
// Created by BoringWednesday on 2021/5/26.
//

#ifndef LIBRTMP_H
#define LIBRTMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "rtmp_packet.h"

struct rtmp_t
{
    int         in_chunk_size;
    int         out_chunk_size;

    struct rtmp_packet_t in_packets[RTMP_MAX_CHUNK_STREAM];
    struct rtmp_packet_t out_packets[RTMP_MAX_CHUNK_STREAM];

};

#if defined(__cplusplus)
}
#endif

#endif //LIBRTMP_H
