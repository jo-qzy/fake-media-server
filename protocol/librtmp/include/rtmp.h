//
// Created by BoringWednesday on 2021/5/26.
//

#ifndef LIBRTMP_H
#define LIBRTMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "rtmp_packet.h"


struct rtmp_handler_t
{
    int (*on_send)(void *param, const uint8_t *header, uint32_t header_bytes, const uint8_t *payload, uint32_t payload_bytes);
};

struct rtmp_t
{
    void       *param;

    int         in_chunk_size;
    int         out_chunk_size;

    struct rtmp_packet_t in_packets[RTMP_MAX_CHUNK_STREAM];
    struct rtmp_packet_t out_packets[RTMP_MAX_CHUNK_STREAM];

    int (*on_send)(void *param, const uint8_t *header, uint32_t header_bytes, const uint8_t *payload, uint32_t payload_bytes);

};


struct rtmp_t *rtmp_create(void *param, const struct rtmp_handler_t *handler);


#if defined(__cplusplus)
}
#endif

#endif //LIBRTMP_H
