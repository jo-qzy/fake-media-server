//
// Created by BoringWednesday on 2021/5/12.
//

#include "rtmp.h"
#include "rtmp_chunk.h"
#include "rtmp_handshake.h"

#include <stdlib.h>
#include <time.h>

/*
 * 1. handshake (p10)
 * C -> S: C0/C1
 * S -> C: S0/S1/S2
 * C -> S: C2
 *
 * 2. connect (p34)
 * C -> S: connect
 * S -> C: Window Acknowledgement Size
 * S -> C: Set Peer Bandwidth
 * C -> S: Window Acknowledgement Size
 * S -> C: User Control Message (StreamBegin)
 * S -> C: _result(connect response)
 *
 * 3. play (p41)
 * C -> S: createStream
 * S -> C: _result(createStream response)
 * C -> S: play
 * C -> S: SetBufferLength (optional)
 * S -> C: SetChunkSize
 * S -> C: User Control (StreamIsRecorded)
 * S -> C: User Control (StreamBegin)
 * S -> C: onStatus (play reset) (only play set reset flag)
 * S -> C: onStatus (play start)
 * S -> C: Audio
 * S -> C: Video
 *
 * 4. publish (p49)
 * C -> S: createStream
 * S -> C: _result(createStream response)
 * C -> S: publish
 * S -> C: User Control (StreamBegin)
 * C -> S: Metadata
 * C -> S: Audio
 * C -> S: SetChunkSize
 * S -> C: onStatus(publish result)
 * C -> S: Video
 */

struct rtmp_t *rtmp_create(void *param, const struct rtmp_handler_t *handler)
{
    struct rtmp_t *ctx;

    ctx = (struct rtmp_t *) calloc(1, sizeof(struct rtmp_t));
    if (!ctx) {
        return NULL;
    }

    ctx->param = param;

    ctx->in_chunk_size = RTMP_CHUNK_SIZE;
    ctx->out_chunk_size = RTMP_CHUNK_SIZE;

    ctx->on_send = handler->on_send;

    return ctx;
}

void rtmp_destroy(struct rtmp_t *rtmp)
{
    if (!rtmp) {
        return;
    }

    free(rtmp);
}

int rtmp_get_status(struct rtmp_t *rtmp)
{
    return rtmp->status;
}

int rtmp_input(struct rtmp_t *rtmp, const void *data, size_t bytes)
{
    const uint8_t *ptr;

    if (data == NULL || bytes == 0) {
        return -1;
    }

    ptr = data;

    while (bytes > 0) {
        switch (rtmp->handshake_status) {
            case RTMP_HANDSHAKE_UNINITIALIZED:

                bytes -= 1;
                break;

            default:
                return rtmp_chunk_read(rtmp, data, bytes);
        }
    }

    return 0;
}

int rtmp_connect(struct rtmp_t *rtmp)
{
    int handshake_size;

    if (rtmp->status > RTMP_STATUS_HANDSHAKE) {
        return -1;
    }

    handshake_size = rtmp_handshake_c0(rtmp->payload, RTMP_VERSION);
    handshake_size += rtmp_handshake_c1(rtmp->payload + 1, (uint32_t) time(NULL));

    return handshake_size == rtmp->on_send(rtmp->param, rtmp->payload, handshake_size, NULL, 0) ? 0 : -1;
}


