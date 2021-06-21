//
// Created by BoringWednesday on 2021/5/26.
//

#ifndef LIBRTMP_H
#define LIBRTMP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "rtmp_packet.h"


enum rtmp_status_t
{
    RTMP_STATUS_UNINITIALIZED   = 0,
    RTMP_STATUS_HANDSHAKE       = 1,
    RTMP_STATUS_CONNECTED       = 2,
    RTMP_STATUS_CREATE_STREAM   = 3,
    RTMP_STATUS_START           = 4,
    RTMP_STATUS_STOP            = 5,
    RTMP_STATUS_DELETE_STREAM   = 6
};

struct rtmp_handler_t
{
    int (*on_send)(void *param, const uint8_t *header, uint32_t header_bytes, const uint8_t *payload, uint32_t payload_bytes);
};

struct rtmp_t
{
    void       *param;

    int         status;
    int         handshake_status;

    int         in_chunk_size;
    int         out_chunk_size;

    uint8_t     payload[2 * 1024];

    struct rtmp_packet_t in_packets[RTMP_MAX_CHUNK_STREAM];
    struct rtmp_packet_t out_packets[RTMP_MAX_CHUNK_STREAM];

    int (*on_send)(void *param, const uint8_t *header, uint32_t header_bytes, const uint8_t *payload, uint32_t payload_bytes);

};


struct rtmp_t *rtmp_create(void *param, const struct rtmp_handler_t *handler);

void rtmp_destroy(struct rtmp_t *rtmp);

int rtmp_get_status(struct rtmp_t *rtmp);

int rtmp_input(struct rtmp_t *rtmp, const void *data, size_t bytes);

int rtmp_connect(struct rtmp_t *rtmp);


#if defined(__cplusplus)
}
#endif

#endif //LIBRTMP_H
